#include <Server.hpp>

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password), _running(true), _socketFd(-1), _epollFd(-1), _maxChannelUsers(15),
	   _serverName ("ircserver.com"), _version("ChatServ-1.0"), _chanModes("itoblk") {}

Server::~Server() {
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second) {
			if (_epollFd >= 0)
				epoll_ctl(_epollFd, EPOLL_CTL_DEL, it->second->getFd(), NULL);
			close(it->second->getFd());
			delete it->second;
		}
	}

	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		if (it->second)
			delete it->second;
	}

	_clients.clear();
	if (_socketFd >= 0)
		close(_socketFd);
	if (_epollFd >= 0)
		close(_epollFd);
}

void Server::init() {
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0) 
		throw std::runtime_error("Creating the socket server");
	/*NOTE: */
	std::cout << "Socket created with fd -> " << _socketFd << std::endl;

	int	opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("When setting SO_REUSEADDR");

	int nodelay = 1;
	if (setsockopt(_socketFd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) < 0)
    	throw std::runtime_error("When setting TCP_NODELAY");

	if (!setNonBlocking(_socketFd))
		throw std::runtime_error("Setting flags to non blocking");

	std::memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET; 
	_servAddr.sin_addr.s_addr = INADDR_ANY; 
	_servAddr.sin_port = htons(_port); 

	if (bind(_socketFd, (sockaddr*)&_servAddr, sizeof(_servAddr)) < 0)
		throw std::runtime_error("When using bind()");

	if (listen(_socketFd, 5) < 0)
		throw std::runtime_error("When using listen()");

	_epollFd = epoll_create1(0);
	if (_epollFd < 0)
		throw std::runtime_error("When creating epoll instance");

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = _socketFd; 
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socketFd, &ev) < 0)
		throw std::runtime_error("When adding the socket to the epoll instance");

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	epoll_event stdin_ev;
	stdin_ev.events = EPOLLIN;
	stdin_ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_ev) < 0)
		throw std::runtime_error("When adding stdin to epoll instance");

	initCmds();
	_creationDate = currentDateTimeString();

	/* NOTE:  11. Print server info */
	std::cout << "Server initialized with the following parameters:" << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
	std::cout << "Socket file descriptor: " << _socketFd << std::endl;
	std::cout << "Epoll file descriptor: " << _epollFd << std::endl;
	std::cout << "Server port (network byte order): " << ntohs(_servAddr.sin_port) << std::endl;
	std::cout << "Server port (host byte order): " << _servAddr.sin_port << std::endl;
	std::cout << "Server address family: " << _servAddr.sin_family << std::endl << std::endl;
	std::cout << "Welcome to the IRC server!" << std::endl;
	std::cout << "Type 'help' for a list of commands." << std::endl;
	std::cout << "Type 'exit' or 'quit' to stop the server." << std::endl;
}

void	Server::connectNewClient()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_fd = accept(_socketFd, (sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
		throw std::runtime_error("Client dont works");

	if (!setNonBlocking(client_fd))
		throw std::runtime_error("Setting flags to non blocking for client");

	int nodelay = 1;
    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

	Client	*auxClient = new Client(client_fd, &client_addr);
	epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP;
	ev.data.fd = client_fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
		throw std::runtime_error("When add new client to epoll");
	_clients.insert(std::pair<int, Client*>(client_fd, auxClient));
}

t_msg	Server::parseMsg(std::string fullMsg)
{
	while (!fullMsg.empty() && (fullMsg[fullMsg.size() - 1] == '\r' || fullMsg[fullMsg.size() - 1] == '\n'))
    	fullMsg.resize(fullMsg.size() - 1);

	t_msg	msg;
	msg.allMsg = fullMsg;

	size_t firstChar = fullMsg.find_first_not_of(' ');
	if (firstChar != std::string::npos)
    	fullMsg = fullMsg.substr(firstChar);

	size_t firstSpace = fullMsg.find(' ');
	std::string args;

	if (firstSpace == std::string::npos)
	{
		msg.command = fullMsg;
		args = "";
	}
	else
	{
		msg.command = fullMsg.substr(0, firstSpace);
		args = fullMsg.substr(firstSpace + 1);
	}

	std::size_t trailingPos = args.find(':');
	if (trailingPos != std::string::npos)
	{
		msg.trailing = args.substr(trailingPos + 1);
		msg.hasTrailing = true;
		args = args.substr(0, trailingPos);
	}

	while(!args.empty() && msg.params.size() < MAX_PARAMS)
	{
		size_t pos = args.find(' ');
		std::string token;

		if (pos == std::string::npos)
		{
			token = args;
			args.clear();
		}
		else
		{
			token = args.substr(0, pos);
			args = args.substr(pos + 1);
		}

		msg.params.push_back(token);
		size_t nonSpace = args.find_first_not_of(' ');

		if (nonSpace != std::string::npos)
			args = args.substr(nonSpace);
		else
			args.clear();
	}

	if (trailingPos == std::string::npos && !args.empty() && msg.params.size() < MAX_PARAMS + 1)
	{
		size_t pos = args.find(' ');
		if (pos == std::string::npos)
			msg.params.push_back(args);
		else
		{
			args = args.substr(0, pos);
			msg.params.push_back(args);
		}
	}
	return msg;
}


void	Server::readMsg(int fd)
{
	char buf[MAX_BYTES_MSG];
    while (true) {
        int bytes = recv(fd, buf, sizeof(buf), 0);
        if (bytes > 0) {
            _clients[fd]->getBufferMsgClient().append(buf, bytes);
        } else if (bytes == 0) {
            disconnectClient(fd);
            return;
        } else {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
                disconnectClient(fd);
            break;
        }
    }
	size_t pos;
    while ((pos = _clients[fd]->getBufferMsgClient().find("\n")) != std::string::npos)
    {
        std::string fullMsg = _clients[fd]->getBufferMsgClient().substr(0, pos);
        _clients[fd]->getBufferMsgClient().erase(0, pos + 1);

        t_msg parsedMsg = parseMsg(fullMsg);
		handleCommand(parsedMsg, fd);
    }
}

void Server::disconnectClient(int fd) {
	if (_clients.find(fd) == _clients.end()) 
		throw std::runtime_error("Trying to disconnect a client that does not exist");
	
	Client *client = _clients[fd];
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		_channel[it->first]->disconnectUser(client);
		if (_channel[it->first]->getUserCount() == 1 && _channel[it->first]->hasUser("Bot"))
			_channel[it->first]->disconnectUser(_clients[_channel[it->first]->getUserFd("Bot")]);
	}
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw std::runtime_error("When removing client from epoll instance");
	if (close(fd) < 0) 
		throw std::runtime_error("When closing client socket");
	delete client;
	_clients.erase(fd);
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		if (_channel[it->first]->getUserCount() == 0) {
			delete _channel[it->first];
			_channel.erase(it);
		}
	}
	/* NOTE: */
	std::cout << GREEN << "Client disconnected successfully." << CLEAR << std::endl;
}

void Server::enableWrite(int fd) {
	if (_clients[fd]->getIsWriting()) 
		return;
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
    ev.data.fd = fd;
    if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) == 0)
		_clients[fd]->setIsWriting(true);
	else
		throw std::runtime_error("Epoll MOD failed in enableWrite");
}

void Server::disableWrite(int fd) {
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP;
    ev.data.fd = fd;
    epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
    _clients[fd]->setIsWriting(false);
}

void Server::handleWrite(int fd) {
    Client *client = _clients[fd];
    
    if (!client->hasPendingMsg()) {
        disableWrite(fd);
        if (client->getShouldDisconnect())
            disconnectClient(fd);
        return;
    }

    const std::string &msg = client->getNextMsg();
    ssize_t bytes_sent = send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
	std::cout << "Enviando: [" << msg << "]" << std::endl;
    if (bytes_sent > 0) {
        client->updateMsg(static_cast<size_t>(bytes_sent));
        
        if (!client->hasPendingMsg()) {
            disableWrite(fd);
            if (client->getShouldDisconnect()) {
                disconnectClient(fd);
                return;
            }
        }
    } 
    else if (bytes_sent < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            disconnectClient(fd);
        }
    }
}

void Server::sendToChannel(const std::string &chanName, int excludeFd, const std::string &msg) {
    if (_channel.find(chanName) == _channel.end())
        return;

    Channel *chan = _channel[chanName];
    chan->broadcastSimple(excludeFd, msg);

    const std::map<int, Client*>& users = chan->getUsers();
    for (std::map<int, Client*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        if (it->first != excludeFd) {
            enableWrite(it->first);
        }
    }
}

void  Server::manageServerInput() {
	std::string input;
	std::getline(std::cin, input);
	if (input.empty())
		return ;
	std::vector<std::string> commands;
	splitCmd(input, commands, ' ');
	if (commands[0] == "exit" || commands[0] == "quit") {
		_running = false;
		/* NOTE: */
		std::cout << PINK << "Server is shutting down..." << CLEAR << std::endl;
	}
	else if (commands[0] == "clients") {
		// TODO: Darle una vuelta a la información facilitada
		std::cout << "Connected clients: " << std::endl;
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			std::cout << "[ Client fd: " << it->first
					  << ", Username: " << it->second->getUsername() 
					  << ", Nickname: " << it->second->getNickname()
					  << " ]" << std::endl;
		}
	}
	else if (commands[0] == "channels") {
		// TODO: Darle una vuelta a la información facilitada
		std::cout << "Available channels: " << std::endl;
		for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
			std::cout << "[ Channel name: " << it->first 
					  << ", Max users: " << it->second->getMaxUsers() 
					  << ", Password: " << it->second->getPass() << " ]"
					  << std::endl;
		}
	}
	else if (commands[0] == "clear" || commands[0] == "cls")
		std::cout << "\033[2J\033[1;1H";
	else if (commands[0] == "limit") {
		if (commands.size() > 1) {
			int number = stringtoint(commands[1]);
			if (number < 1 || number > 100)
				std::cout << RED << "Please choose a number between 1 and 100" << CLEAR << std::endl;
			else
				_maxChannelUsers = number;
		}
		std::cout << "Max channels per user: " << _maxChannelUsers << std::endl;
	}
	else if (commands[0] == "version")
		std::cout << "Server version: 1.0.0" << std::endl;
	else if (commands[0] == "help") {
		std::cout << "Available commands:" << std::endl;
		std::cout << "- exit/quit: Stop the server." << std::endl;
		std::cout << "- clients: List connected clients." << std::endl;
		std::cout << "- channels: List channels." << std::endl;
		std::cout << "- clear/cls: Clear the console." << std::endl;
		std::cout << "- limit: Show max channels per user." << std::endl;
		std::cout << "- version: Show server version." << std::endl;
		std::cout << "- help: Show this help message." << std::endl;
	}
	else
		std::cout << RED << "Command not recognized. Type 'help' for a list of commands." << CLEAR << std::endl;
}

void Server::run() {
	epoll_event events[MAX_EVENTS];

	while (_running) {
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents < 0) {
			if (errno == EINTR) {
				std::cout << "Closing server by signal..." << std::endl;
				_running = false;
				continue;
			}
			throw std::runtime_error("When waiting for events");
		}
		try {
			for (int i = 0; i < numEvents; i++) {
				if (events[i].data.fd == STDIN_FILENO) 
					manageServerInput();
				else if (events[i].data.fd == _socketFd)
					connectNewClient();
				else {
					int fd = events[i].data.fd;

					if (events[i].events & EPOLLIN)
						readMsg(fd);
					if (events[i].events & EPOLLOUT) {
						if (_clients.find(fd) == _clients.end())
							continue;
						handleWrite(fd);
						epoll_event ev;
						ev.events = EPOLLIN | EPOLLRDHUP;
						ev.data.fd = fd;
						if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) < 0)
							throw std::runtime_error("When modifying client to epoll instance after write");
						_clients[fd]->setHasPendingMsg(false);
					}
					if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
						disconnectClient(fd);
				}
			}
		}
		catch (const std::exception &e) {
			std::cerr << RED << "Error: " << CLEAR << e.what() << std::endl;
		}
	}
}
