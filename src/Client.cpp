#include <Client.hpp>

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

// GETTERS

const std::string &Client::getUsername() const { return _username; }

const std::string &Client::getNickname() const { return _nickname; }

const std::string &Client::getRealname() const { return _realname; }

const std::string &Client::getPassword() const { return _password; }

int Client::getFd() const { return _fd; }

// SETTERS

void Client::setUsername(const std::string &username) { _username = username; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }

void Client::setRealname(const std::string &realname) { _realname = realname; }

void Client::setPassword(const std::string &password) { _password = password; }

void Client::setFd(int fd) { _fd = fd; }


