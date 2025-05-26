#include <Server.hpp>

Server::~Server() {
}

Server::Server() {
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

