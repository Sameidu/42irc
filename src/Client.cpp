#include "../includes/Client.hpp"

Client::~Client() {
}

Client::Client() {
}

Client::Client(const Client &other) {
	*this = other;
}


Client &Client::operator=(const Client &other) {
	if ( this != &other ) {
		
	}
	return *this;
}

