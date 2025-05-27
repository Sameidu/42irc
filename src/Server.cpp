#include <Server.hpp>

Server::~Server() {
}

Server::Server( const int &port, const std::string &password )
	: _port(port), _password(password) {
}

Server::Server( const Server &other ) {
	*this = other;
}


Server &Server::operator=( Server const &other )
{
	if ( this != &other ) {

	}
	return *this;
}

