#include <Server.hpp>

Server::~Server() {
}

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password) {
		std::cout << "Initializing server on port " << _port << " with password '" << _password << "'." << std::endl;
		std::cout << "Server initialized successfully." << std::endl;

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

