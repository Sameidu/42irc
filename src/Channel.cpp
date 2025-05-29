#include <Channel.hpp>

Channel::~Channel() {
}

Channel::Channel() {
}

Channel::Channel(const Channel &other) {
	*this = other;
}


Channel &Channel::operator=(const Channel &other) {
	if ( this != &other ) {
		
	}
	return *this;
}

// GETTERS

const std::string &Channel::getName() const { return _name; }

const int &Channel::getMaxUsers() const { return _maxUsers; }

// SETTERS

void Channel::setName(const std::string &name) { _name = name; }

void Channel::setMaxUsers(const int &maxUsers) { _maxUsers = maxUsers; }

