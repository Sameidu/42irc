#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password), _running(true), _socketFd(-1), _epollFd(-1) {}

Server::~Server() {
	if (_socketFd >= 0)
		close(_socketFd);
	if (_epollFd >= 0)
		close(_epollFd);
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second) {
			close(it->second->getFd());
			delete it->second;
		}
	}
	_clients.clear();
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

	/* NOTE:  9. Print server info */
	std::cout << "Server initialized with the following parameters:" << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
	std::cout << "Socket file descriptor: " << _socketFd << std::endl;
	std::cout << "Epoll file descriptor: " << _epollFd << std::endl;
	std::cout << "Server port (network byte order): " << ntohs(_servAddr.sin_port) << std::endl;
	std::cout << "Server port (host byte order): " << _servAddr.sin_port << std::endl;
	std::cout << "Server address family: " << _servAddr.sin_family << std::endl;
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

	Client	*auxClient =	new Client(client_fd, &client_addr);
	epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP; // EPOLLRDHUP to handle client disconnection
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
	close(fd);
	_clients.erase(fd);
	
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	std::cout << "Client disconnected successfully." << std::endl;
}

void Server::run() {
	epoll_event events[MAX_EVENTS];

	while (_running) {
		// TODO: Hay que manejar señales en el server.
		std::cout<< "Waiting for events..." << std::endl << std::endl;
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents < 0)
			throw std::runtime_error("Error: when waiting for events");
		
		try {
			for (int i = 0; i < numEvents; i++) {
				if (events[i].data.fd == _socketFd)
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
