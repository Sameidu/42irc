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

