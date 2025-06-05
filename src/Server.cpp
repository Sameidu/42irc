#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

Server::~Server() {
}

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password) {
		std::cout << "Initializing server on port " << _port << " with password '" << _password << "'." << std::endl;
		std::cout << "Server initialized successfully." << std::endl;
		
		_port = port;
		_password = password;

		/* 1. IPv4 (AF_INET) and TCP (SOCK_STREAM) */
		_socketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (_socketFd < 0) 
			throw std::runtime_error("Error: creating the socket server");

		/* 2. Allows the port to be reused immediately after closing the socket */
		/* 	- The level argument specifies the protocol level at which the option resides. 
				To set options at the socket level, specify the level argument as SOL_SOCKET.
			- SO_REUSEADDR
				Specifies that the rules used in validating addresses supplied to bind() should allow reuse of local addresses, 
				if this is supported by the protocol. This option takes an int value. This is a Boolean option.
		*/
		int	opt = 1;
		if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			close(_socketFd); /* TODO: cerramos aqui, en el destructor ? */
			throw std::runtime_error("Error: when setting SO_REUSEADDR");
		}

		/* 3. change the fd flags to make the file non-blocking ---> esta en utils.cpp la funcion*/
		if (!setNonBlocking(_socketFd))
			throw std::runtime_error("Error: setting flags to non blocking");

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
		{
			close(_socketFd); /* TODO: cerramos aqui, en el destructor ? */
			throw std::runtime_error("Error: when using bind()");
		}

		/* 6. Put the socket in listening mode
		Puts the socket in waiting mode. The number 5 is the maximum waiting queue size.*/
		/* TODO: he puesto en 5 el numero maximo de conexiones que pueden estar en la cola pero no se que es mejor
		cuantos clientes deberian poder intentarse conectar al mismo tiempo??*/
		if (listen(_socketFd, 5) < 0)
		{
			close(_socketFd); /* TODO: cerramos aqui, en el destructor ? */
			throw std::runtime_error("Error: when using listen()");
		}

		std::cout << "Server listen in port -> " << port << std::endl;

		/* 7. create poll struct */
		_fds[0].fd = _socketFd; // metemos el socket del server
		_fds[0].events = POLLIN; // lo que tiene que vigilar , si el socket escribe en el
		_nFds = 1; // esto es un contador de cuantos sockets tiene que vigilar el poll se inicia a uno por que esta el server

}

Server::Server( const Server &other ) {
	*this = other;
}


Server &Server::operator=( Server const &other )
{
	if ( this != &other ) {
;
	}
	return *this;
}


void Server::run()
{
	while(1)
	{

	}
}

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

void Server::setPort( const int &port ) { _port = port; }

void Server::setPassword( const std::string &password ) { _password = password; }

