#include <Channel.hpp>

Channel::~Channel() {}

Channel::Channel(const std::string &name) : _maxUsers(0), _name(name), _ownerFd(-1) {}

// GETTERS

const std::string &Channel::getName() const { return _name; }

const std::string &Channel::getPass() const { return _pass; }

const size_t &Channel::getMaxUsers() const { return _maxUsers; }

const std::string &Channel::getTopic() const { return _topic; }

size_t Channel::getUserCount() const { return _users.size(); }

int Channel::getUserFd(const std::string &nick) const {
	for (std::map<int, Client *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->second->getNickname() == nick)
			return it->first;
	}
	return -1; // Si no se encuentra el usuario, se devuelve -1
}

std::string Channel::getMode() const {
	std::string modes = "+";
	for (std::set<char>::const_iterator it = _mode.begin(); it != _mode.end(); ++it) {
		modes += *it;
	}
	return modes;
}

// SETTERS

void Channel::setName(const std::string &name) { _name = name; }

void Channel::setPass(const std::string &pass) { _pass = pass; }

void Channel::setMaxUsers(const size_t &maxUsers) { _maxUsers = maxUsers; }

void Channel::setMode(const char &mode) { _mode.insert(mode); }

void Channel::unsetMode(const char &mode) { _mode.erase(mode); }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

// CHECKS

bool Channel::isBanned(int fd) const { return _banned.find(fd) != _banned.end(); }

bool Channel::isInvited(int fd) const { return _invited.find(fd) != _invited.end(); }

bool Channel::isAdmin(int fd) const { return _admins.find(fd) != _admins.end(); }

bool Channel::hasMode(const char &mode) const { return _mode.find(mode) != _mode.end(); }

bool Channel::hasUser(int fd) const { return _users.find(fd) != _users.end(); }

bool Channel::hasUser(const std::string &nick) const {
	for (std::map<int, Client *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->second->getNickname() == nick)
			return true;
	}
	return false;
}

// METHODS

void Channel::newChannelUser(Client *client) {
	// Error si el canal está ya lleno por limite de usarios
	if (_users.size() >= _maxUsers && _maxUsers > 0)
		throw std::runtime_error("Channel is full.");
	// Error si el cliente que solicita conectarse ya esta en el canal
	if (_users.find(client->getFd()) != _users.end())
		throw std::runtime_error("User already connected to this channel.");
	// Error en el caso de que el usuario este banneado del canal
	if (_banned.find(client->getFd()) != _banned.end())
		throw std::runtime_error("User is banned from this channel.");
	// Si no hay propietario, se asigna el primer cliente como tal
	if (_ownerFd == -1) {
		_ownerFd = client->getFd();
		_admins.insert(std::make_pair(client->getFd(), client));
	}
	// En cualquier caso, se añade el cliente a la lista de usuarios del canal
	_users.insert(std::make_pair(client->getFd(), client));
	client->joinChannel(this);
	std::cout << "User " << client->getNickname() << " connected to channel " << _name << std::endl;
}

void Channel::disconnectUser(Client *client) {
	if (_users.find(client->getFd()) == _users.end())
		return ;

	if (_users.find(client->getFd()) != _users.end()) {
		if (_ownerFd == client->getFd()) {
			if (_admins.size() > 1) {
				removeAdminList(client);
				if (_admins.begin()->second->getNickname() != "Bot")
					_ownerFd = _admins.begin()->first;
			}
			else
			_ownerFd = -1;
		}
	}
	if (_ownerFd == -1 && _users.size() > 1) {
		for (std::map<int, Client *>::iterator it = _users.begin(); it != _users.end(); ++it) {
			if (it->second->getFd() != client->getFd()) {
				if (it->second->getNickname() != "Bot") {
					_ownerFd = it->first;
					break;
				}
			}
		}
		if (_ownerFd != -1) {
			_admins.insert(std::make_pair(_ownerFd, _users[_ownerFd]));
			std::string reponse = ":" + _users[_ownerFd]->getNickname() + "!" + _users[_ownerFd]->getUsername() + "@localhost";
			reponse += " MODE " + _name + " +o " + _users[_ownerFd]->getNickname() + "\r\n";
			if (send(_ownerFd, reponse.c_str(), reponse.size(), MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
			broadcastMessage(client->getFd(), "MODE", _users[_ownerFd]->getNickname(), "+o " + _users[_ownerFd]->getNickname());
		}
	}
	if (isAdmin(client->getFd()))
		removeAdminList(client);
	if (isInvited(client->getFd()))
		removeInvitedList(client);
	client->leaveChannel(this);
	_users.erase(client->getFd());
}

void Channel::addInvitedList(Client *client) {
	if (_invited.find(client->getFd()) != _invited.end())
		throw std::runtime_error("User already invited to this channel.");
	_invited.insert(std::make_pair(client->getFd(), client));
}

void Channel::removeInvitedList(Client *client) {
	if (_invited.find(client->getFd()) == _invited.end())
		throw std::runtime_error("User not invited to this channel.");
	_invited.erase(client->getFd());
}

void Channel::addBannedList(Client *client) {
	if (_banned.find(client->getFd()) != _banned.end())
		throw std::runtime_error("User already banned from this channel.");
	_banned.insert(std::make_pair(client->getFd(), client));
}

void Channel::removeBannedList(Client *client) {
	if (_banned.find(client->getFd()) == _banned.end())
		throw std::runtime_error("User not banned from this channel.");
	_banned.erase(client->getFd());
}

void Channel::addAdminList(Client *client) {
	if (_admins.find(client->getFd()) != _admins.end())
		throw std::runtime_error("User already admin of this channel.");
	_admins.insert(std::make_pair(client->getFd(), client));
}

void Channel::removeAdminList(Client *client) {
	if (_admins.find(client->getFd()) == _admins.end())
		throw std::runtime_error("User not admin of this channel.");
	_admins.erase(client->getFd());
	_invited.erase(client->getFd());
}

void Channel::broadcastMessageNochan(int fd, const std::string &cmd, const std::string &msg) const {
	std::string prefix = ":" + _users.at(fd)->getNickname() + "!" + _users.at(fd)->getUsername() + "@localhost";
	std::string message = prefix + " " + cmd;
	
	if (!msg.empty())
		message += " :" + msg;
	message += "\r\n";
	for (std::map<int, Client *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->first != fd)
			if (send(it->first, message.c_str(), message.size(), MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
	}
}

void Channel::broadcastMessage(int fd, const std::string &cmd, const std::string &user, const std::string &msg) const {
	std::string prefix = ":" + _users.at(fd)->getNickname() + "!" + _users.at(fd)->getUsername() + "@localhost";
	std::string message = prefix + " " + cmd + " " + _name;
	
	if (!user.empty())
		message += " " + user;
	if (!msg.empty())
		message += " :" + msg;
	message += "\r\n";
	for (std::map<int, Client *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->first != fd)
			if (send(it->first, message.c_str(), message.size(), MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
	}
}

std::string Channel::listUsers() {
	std::string list;
	if (_users.empty())
		return list;
	for (std::map<int, Client *>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if (!list.empty())
			list += " ";
		if (isAdmin(it->first))
			list += "@";
		list += it->second->getNickname();
	}
	return list;
}

std::string Channel::listBanned() {
	std::string list;
	if (_banned.empty())
		return list;
	for (std::map<int, Client *>::iterator it = _banned.begin(); it != _banned.end(); ++it) {
		if (!list.empty())
			list += " ";
		list += it->second->getNickname();
	}
	return list;
}
