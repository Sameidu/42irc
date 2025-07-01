#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password), _running(true), _socketFd(-1), _epollFd(-1), _maxChannelUsers(15) {}

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

	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		if (it->second)
			delete it->second;
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

	// Channel *generalChannel = new Channel("general");
	// _channel.insert(std::pair<std::string, Channel *>("general", generalChannel));

	/* 9. Add stdin to epoll instance */
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	epoll_event stdin_ev;
	stdin_ev.events = EPOLLIN;
	stdin_ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_ev) < 0)
		throw std::runtime_error("Error: when adding stdin to epoll instance");

	/* 10. Create std::map for commands */
	initCmds();

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

t_msg	Server::parseMsg(std::string fullMsg)
{
	std::cout << "Mensaje recibido:\n" << BLUE << fullMsg << CLEAR << std::endl;

	/* 1. Me aseguro de que no quedan \r o \n por el msg los mando al final y los borro */
	while (!fullMsg.empty() && (fullMsg[fullMsg.size() - 1] == '\r' || fullMsg[fullMsg.size() - 1] == '\n'))
    	fullMsg.resize(fullMsg.size() - 1);

	t_msg	msg;

	/* guardo todo el msg */
	msg.allMsg = fullMsg;

	/* 2. elimina espacios iniciales */
	size_t firstChar = fullMsg.find_first_not_of(' ');
	if (firstChar != std::string::npos)
    	fullMsg = fullMsg.substr(firstChar);

	/* 3. busca primer espacio y guarda la primera palabra que deberia de ser el comando */
	size_t firstSpace = fullMsg.find(' ');
	std::string args;

	if (firstSpace == std::string::npos) // solo te mandan un comando sin args
	{
		msg.command = fullMsg;
		args = "";
	}
	else
	{
		msg.command = fullMsg.substr(0, firstSpace);
		/* 4. quito el comando y guardo el resto como agumentos */
		args = fullMsg.substr(firstSpace + 1);
	}

	/* 5. busco si tiene trailing ':' y lo separo */
	std::size_t trailingPos = args.find(':');
	if (trailingPos != std::string::npos)
	{
		msg.trailing = args.substr(trailingPos + 1);
		msg.hasTrailing = true;
		args = args.substr(0, trailingPos); // quito el trailing si hay 
	}
	
	/* 6. mientras haya args y sean menos a 13 (max params)*/

	while(!args.empty() && msg.params.size() < 13)
	{
		size_t pos = args.find(' ');
		std::string token; // guardamos el siguiente param

		// Si no hemos encontrado más espacios → el resto del string es un token completo
		if (pos == std::string::npos)
		{
			token = args; // Todo args es el token
			args.clear(); // Ya no queda nada por procesar
		}
		else
		{
			// Hemos encontrado un espacio → el token es lo que hay antes del espacio
			token = args.substr(0, pos);
			// Quitamos el token y el espacio
			args = args.substr(pos + 1);
		}

		// Añadimos el token al vector de params
		msg.params.push_back(token);

		// Saltamos los espacios en blanco que puedan quedar al principio de args
		size_t nonSpace = args.find_first_not_of(' ');

		if (nonSpace != std::string::npos)
			args = args.substr(nonSpace); // Quitamos los espacios
		else
			args.clear(); // Si solo quedaban espacios → vaciamos args
	}

	/* si no tiene trailing el protocolo admite 14 parametros si hay mas los ignoro y los pierdo */
	if (trailingPos == std::string::npos && !args.empty() && msg.params.size() < 14)
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
	std::cout << "Evento recibido de fd: " << fd << std::endl;

	char	msg[MAX_BYTES_MSG];
	std::memset(msg, 0, sizeof(msg));

	int bytes_recived =  recv(fd, &msg, MAX_BYTES_MSG, 0);

	if (bytes_recived < 0)
		throw std::runtime_error("Error: on recv()");

	std::cout << "Numero de bytes recibidos en el msg: " << bytes_recived << std::endl;

	// Acumular siempre lo recibido en el buffer del cliente
    std::string aux = _clients[fd]->getBufferMsgClient();
    aux.append(msg, bytes_recived); // Usar bytes_recived, por si hay '\0' intermedios
    _clients[fd]->setBufferMsgClient(aux);

    std::string& buffer = _clients[fd]->getBufferMsgClient();
    std::size_t pos;

    // Mientras haya un mensaje completo (terminado en "\r\n")
    while ((pos = buffer.find("\n")) != std::string::npos)
    {
        std::string fullMsg = buffer.substr(0, pos); // ya no esta \r\n
        buffer.erase(0, pos + 1); // Eliminar mensaje + "\r\n" para seguir el bucle

        std::cout << YELLOW << "REAL MSG: " << fullMsg << CLEAR << std::endl;

        t_msg parsedMsg = parseMsg(fullMsg); // Parsear el mensaje
		handleCommand(parsedMsg, fd);
    }
}

void Server::disconnectClient(int fd) {
	if (_clients.find(fd) == _clients.end()) 
		throw std::runtime_error("Error: trying to disconnect a client that does not exist");
	
	std::cout << "Disconnecting client with fd: " << fd << std::endl;
	Client *client = _clients[fd];
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it)
		_channel[it->first]->disconnectUser(client);	
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw std::runtime_error("Error: when removing client from epoll instance");
	if (close(fd) < 0) 
		throw std::runtime_error("Error: when closing client socket");
	delete client;
	_clients.erase(fd);
	std::cout << GREEN << "Client disconnected successfully." << CLEAR << std::endl;
}

void  Server::manageServerInput() {
	std::string input;
	std::getline(std::cin, input);
	if (input.empty())
		return;
	if (input == "exit" || input == "quit") {
		_running = false;
		std::cout << PINK << "Server is shutting down..." << CLEAR << std::endl;
	}
	else if (input == "clients") {
		// TODO: Darle una vuelta a la información facilitada
		std::cout << "Connected clients: " << std::endl;
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			std::cout << "[ Client fd: " << it->first
					  << ", Username: " << it->second->getUsername() 
					  << ", Nickname: " << it->second->getNickname() 
					  << ", Realname: " << it->second->getRealname() << " ]"
					  << std::endl;
		}
	}
	else if (input == "channels") {
		// TODO: Darle una vuelta a la información facilitada
		std::cout << "Available channels: " << std::endl;
		for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
			std::cout << "[ Channel name: " << it->first 
					  << ", Max users: " << it->second->getMaxUsers() 
					  << ", Password: " << it->second->getPass() << " ]"
					  << std::endl;
		}
	}
	else if (input == "clear" || input == "cls")
		std::cout << "\033[2J\033[1;1H"; // Clear the console
	else if (input == "limit") {
		// Este va a ser solo para ver el limite actual
		// TODO: Crear setLimit para modificar el valor (Actualizar a todos los usuarios conectados)
		std::cout << "Max channels per user: " << _maxChannelUsers << std::endl;
	}
	else if (input == "version") {
		std::cout << "Server version: 1.0.0" << std::endl;
	}
	else if (input == "add_admin") {
		// Este y el siguiente no se si los veo necesarios
	}
	else if (input == "del_admin") {
	}
	else if (input == "help") {
		std::cout << "Available commands:" << std::endl;
		std::cout << "- exit/quit: Stop the server." << std::endl;
		std::cout << "- clients: List connected clients." << std::endl;
		std::cout << "- channels: List channels." << std::endl;
		std::cout << "- clear/cls: Clear the console." << std::endl;
		std::cout << "- limit: Show max channels per user." << std::endl;
		std::cout << "- version: Show server version." << std::endl;
		std::cout << "- add_admin: Add an admin to a channel." << std::endl;
		std::cout << "- del_admin: Remove an admin from a channel." << std::endl;
		std::cout << "- help: Show this help message." << std::endl;
	}
	else {
		std::cout << RED << "Command not recognized. Type 'help' for a list of commands." << CLEAR << std::endl;
	}
}

void Server::run() {
	epoll_event events[MAX_EVENTS];

	while (_running) {
		std::cout << PINK << "Waiting for events..." << CLEAR << std::endl << std::endl;
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
			std::cerr << RED << "ErrorUwU: " << CLEAR << e.what() << std::endl;
		}
	}
}
