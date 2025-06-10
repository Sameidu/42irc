#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password), _running(true), _socketFd(-1), _epollFd(-1) {}

Server::~Server() {
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second) {
			std::cout << "Closing client with fd: " << it->second->getFd() << std::endl;
			if (_epollFd >= 0)
				epoll_ctl(_epollFd, EPOLL_CTL_DEL, it->second->getFd(), NULL);
			close(it->second->getFd());
			delete it->second;
		}
	}
	_clients.clear();
	if (_socketFd >= 0)
		close(_socketFd);
	if (_epollFd >= 0)
		close(_epollFd);
	std::cout << "Server closed" << std::endl;
}

void Server::init() {
	/* 1. IPv4 (AF_INET) and TCP (SOCK_STREAM) */
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0) 
		throw std::runtime_error("Error: creating the socket server");
	std::cout << "Socket created with fd -> " << _socketFd << std::endl;
	/* 2. Allows the port to be reused immediately after closing the socket */
	int	opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Error: when setting SO_REUSEADDR");
	else
		std::cout << "SO_REUSEADDR set successfully" << std::endl;
	/* 3. change the fd flags to make the file non-blocking ---> esta en utils.cpp la funcion*/
	// TODO: Revisar fcntl() acorde con subject
	if (!setNonBlocking(_socketFd))
		throw std::runtime_error("Error: setting flags to non blocking");
	else
		std::cout << "Socket set to non-blocking mode" << std::endl;
	/* 4. Init values at addr server struct */
	std::memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET; 
	_servAddr.sin_addr.s_addr = INADDR_ANY; 
	_servAddr.sin_port = htons(_port); 
			
	/* 5. Associate a socket with the IP address and port */
	if (bind(_socketFd, (sockaddr*)&_servAddr, sizeof(_servAddr)) < 0)
		throw std::runtime_error("Error: when using bind()");
	/* 6. Put the socket in listening mode
	// TODO: he puesto en 5 el numero maximo de conexiones que pueden estar en la cola pero no se que es mejor
	cuantos clientes deberian poder intentarse conectar al mismo tiempo??*/
	if (listen(_socketFd, 5) < 0)
		throw std::runtime_error("Error: when using listen()");

	/* 7. Create epoll instance */
	_epollFd = epoll_create1(0);
	if (_epollFd < 0)
		throw std::runtime_error("Error: when creating epoll instance");

	/* 8. Add the socket to the epoll instance */
	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = _socketFd; 
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socketFd, &ev) < 0)
		throw std::runtime_error("Error: when adding the socket to the epoll instance");

	/* 9. Add stdin to epoll instance */
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	epoll_event stdin_ev;
	stdin_ev.events = EPOLLIN;
	stdin_ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_ev) < 0)
		throw std::runtime_error("Error: when adding stdin to epoll instance");

	/* NOTE:  10. Print server info */
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
		throw std::runtime_error("El cliente no furula");

	if (!setNonBlocking(client_fd))
		throw std::runtime_error("Error: setting flags to non blocking for client");

	Client	*auxClient = new Client(client_fd, &client_addr);
	epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP;
	ev.data.fd = client_fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
		throw std::runtime_error("Error: when add new client to epoll");
	_clients.insert(std::pair<int, Client*>(client_fd, auxClient));

	// NOTE: borrar
	std::cout << "Cliente conectado con fd: " << client_fd << std::endl;
	std::cout << "Cliente conectado desde: " 
	<< inet_ntoa(client_addr.sin_addr) << ":" 
	<< ntohs(client_addr.sin_port) << std::endl;
}
void	Server::parseMsg(std::string msg, int fdClient)
{
	std::cout << "hola: " <<  _clients[fdClient]->getFd() << std::endl;
	std::cout << "Mensaje recibido: " << msg << std::endl;
}

void	Server::readMsg(int fd)
{
	std::cout << "Evento recibido de fd: " << fd << std::endl;
	char	msg[MAX_BYTES_MSG];
	std::memset(msg, 0, sizeof(msg));
	int bytes_recived =  recv(fd, &msg, MAX_BYTES_MSG, 0);
	std::cout << bytes_recived << std::endl;
	if (bytes_recived < 0)
		throw std::runtime_error("Error: on recv()");
	// NOTE: borrar
	parseMsg(msg, fd);

}

void Server::disconnectClient(int fd) {
	if (_clients.find(fd) == _clients.end()) 
		throw std::runtime_error("Error: trying to disconnect a client that does not exist");
	
	std::cout << "Disconnecting client with fd: " << fd << std::endl;
	Client *client = _clients[fd];
	if (close(fd) < 0) 
		throw std::runtime_error("Error: when closing client socket");
	delete client;
	_clients.erase(fd);

	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw std::runtime_error("Error: when removing client from epoll instance");
	std::cout << "Client disconnected successfully." << std::endl;
}

void  Server::manageServerInput() {
	std::string input;
	std::getline(std::cin, input);
	if (input.empty())
		return;
	if (input == "exit" || input == "quit") {
		_running = false;
		std::cout << "Server is shutting down..." << std::endl;
	}
	else if (input == "clients") {
		std::cout << "Connected clients: " << std::endl;
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			std::cout << "[ Client fd: " << it->first
					  << ", Username: " << it->second->getUsername() 
					  << ", Nickname: " << it->second->getNickname() 
					  << ", Realname: " << it->second->getRealname() << " ]"
					  << std::endl;
		}
	}
	else if (input == "help") {
		std::cout << "Available commands:" << std::endl;
		std::cout << "- exit/quit: Stop the server." << std::endl;
		std::cout << "- clients: List connected clients. (Esto furula a medias)" << std::endl;
		std::cout << "- channels: List channels. (Mentira, no funciona de momento)" << std::endl;
	}
	else {
		std::cout << "Command not recognized. Type 'help' for a list of commands." << std::endl;
	}
}

void Server::run() {
	epoll_event events[MAX_EVENTS];

	while (_running) {
		// TODO: Hay que manejar señales en el server.
		std::cout<< "Waiting for events..." << std::endl << std::endl;
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents < 0) {
			if (errno == EINTR) {
				std::cout << "Closing server by signal..." << std::endl;
				_running = false;
				continue;
			}
			throw std::runtime_error("Error: when waiting for events");
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
					if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
						disconnectClient(fd);
					// TODO: gestionar el resto de eventos
				}
			}
		}
		catch (const std::exception &e) {
			std::cerr << RED << "Error: " << CLEAR << e.what() << std::endl;
		}
	}
}
