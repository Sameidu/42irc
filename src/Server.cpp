#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password), _socketFd(-1), _epollFd(-1) {}
	
Server::~Server() {
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
	/* 	- The level argument specifies the protocol level at which the option resides. 
			To set options at the socket level, specify the level argument as SOL_SOCKET.
		- SO_REUSEADDR
			Specifies that the rules used in validating addresses supplied to bind() should allow reuse of local addresses, 
			if this is supported by the protocol. This option takes an int value. This is a Boolean option.
	*/
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
	_servAddr.sin_family = AF_INET; // Specifies that the socket will use IPv4 addresses
	_servAddr.sin_addr.s_addr = INADDR_ANY; /*INADDR_ANY is a special constant in socket 
	programming that means, Listen on all available network interfaces of this host, 
	no matter which interface the connection comes from, 
	it is accepted as long as it reaches the specified port. */
	_servAddr.sin_port = htons(_port); //port (converted to network format)
			
	/* 5. Associate a socket with the IP address and port */
	/* (sockaddr*)&_servAddr --> Bind usa sockaddr no sockaddr_in que es el tipo de estructura que usamos 
	 entonces la tenemos que castear a sockaddr* y puntero por que espera un puntero
	 a la estructura no una estructura como tal*/
	if (bind(_socketFd, (sockaddr*)&_servAddr, sizeof(_servAddr)) < 0)
		throw std::runtime_error("Error: when using bind()");
	/* 6. Put the socket in listening mode
	Puts the socket in waiting mode. The number 5 is the maximum waiting queue size.*/
	/* TODO: he puesto en 5 el numero maximo de conexiones que pueden estar en la cola pero no se que es mejor
	cuantos clientes deberian poder intentarse conectar al mismo tiempo??*/
	if (listen(_socketFd, 5) < 0)
		throw std::runtime_error("Error: when using listen()");

	/* 7. Create epoll instance */
	_epollFd = epoll_create1(0);
	if (_epollFd < 0)
		throw std::runtime_error("Error: when creating epoll instance");

	/* 8. Add the socket to the epoll instance */
	epoll_event ev;
	ev.events = EPOLLIN; // We want to monitor for incoming connections
	ev.data.fd = _socketFd; // The data field can be used to store the file descriptor
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socketFd, &ev) < 0)
		throw std::runtime_error("Error: when adding the socket to the epoll instance");

	/* 9. Print server info */
	std::cout << "Server initialized with the following parameters:" << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
	std::cout << "Socket file descriptor: " << _socketFd << std::endl;
	std::cout << "Epoll file descriptor: " << _epollFd << std::endl;
	std::cout << "Server port (network byte order): " << ntohs(_servAddr.sin_port) << std::endl;
	std::cout << "Server port (host byte order): " << _servAddr.sin_port << std::endl;
	std::cout << "Server address family: " << _servAddr.sin_family << std::endl;
}

void Server::run() {
	epoll_event events[MAX_EVENTS];

	while (true) {
		// TODO: Hay que manejar señales en el server.
		std::cout<< "Waiting for events..." << std::endl << std::endl;
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents < 0)
			throw std::runtime_error("Error: when waiting for events");
		try {
			for (int i = 0; i < numEvents; i++) {
				if (events[i].data.fd == _socketFd) {
					// Esto es una nueva conexion de cliente al socket del servidor
					sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);

				 	int client_fd = accept(_socketFd, (sockaddr*)&client_addr, &client_len);
					if (client_fd < 0)
						throw std::runtime_error("El cliente no furula");
					if (!setNonBlocking(client_fd))
					throw std::runtime_error("Error: setting flags to non blocking for client");
				std::cout << "Cliente conectado con fd: " << client_fd << std::endl;
				std::cout << "Cliente conectado desde: " 
					<< inet_ntoa(client_addr.sin_addr) << ":" 
					<< ntohs(client_addr.sin_port) << std::endl;
					// Falta en realidad crear el objeto y guardar bien sus datos para luego poder manejar sus peticiones y tal
				}
				else {
					// Y si recibe cualquier otro evento que no sea el del socket del servidor hacemos cosas con los clientes (Aqui tambien cerrariamos clientes si se mueren)
					std::cout << "Evento recibido de fd: " << events[i].data.fd << std::endl;
				}
			}
		}
		catch (const std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
}
