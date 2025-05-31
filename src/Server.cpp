#include <Server.hpp>

/* TODO: Como gestionamos errores con try catch¿?¿ de momento estoy mandando cuando falla con throw*/

Server::~Server() {
}

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password) {
		std::cout << "Initializing server on port " << _port << " with password '" << _password << "'." << std::endl;
		std::cout << "Server initialized successfully." << std::endl;
	
		/* IPv4 (AF_INET) and TCP (SOCK_STREAM) */
		_socketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (_socketFd < 0) 
			throw std::runtime_error("Error: creating the socket server");

		/* Allows the port to be reused immediately after closing the socket */
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

		if (!setNonBlocking(_socketFd))
			throw std::runtime_error("Error: setting flags to non blocking");

		/* Init values at addr server struct */
		std::memset(&_servAddr, 0, sizeof(_servAddr));
		_servAddr.sin_family = AF_INET; // Specifies that the socket will use IPv4 addresses
		_servAddr.sin_addr.s_addr = /* TODO: */ ;
		_servAddr.sin_port = /* TODO: */;
		
 		
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

const int &Server::getPort() const { return _port; }

const std::string &Server::getPassword() const { return _password; }

void Server::setPort( const int &port ) { _port = port; }

void Server::setPassword( const std::string &password ) { _password = password; }

