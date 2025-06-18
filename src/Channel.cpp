#include <Channel.hpp>

Channel::~Channel() {}

Channel::Channel(const std::string &name) : _maxUsers(0), _name(name), _ownerFd(-1) {}

// GETTERS

const std::string &Channel::getName() const { return _name; }

const std::string &Channel::getPass() const { return _pass; }

const size_t &Channel::getMaxUsers() const { return _maxUsers; }

size_t Channel::getUserCount() const { return _users.size(); }

// SETTERS

void Channel::setName(const std::string &name) { _name = name; }

void Channel::setPass(const std::string &pass) { _pass = pass; }

void Channel::setMaxUsers(const size_t &maxUsers) { _maxUsers = maxUsers; }

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
	if (!_ownerFd) {
		_ownerFd = client->getFd();
		_admins.insert(std::make_pair(client->getFd(), client));
	}
	// En cualquier caso, se añade el cliente a la lista de usuarios del canal
	_users.insert(std::make_pair(client->getFd(), client));
	std::cout << "User " << client->getNickname() << " connected to channel " << _name << std::endl;
}

void Channel::disconnectUser(Client *client) {
	if (_users.find(client->getFd()) == _users.end())
		return ;

	if (_admins.find(client->getFd()) != _admins.end()) {
		if (_ownerFd == client->getFd()) {
			// Si el propietario se desconecta, se asigna un nuevo propietario a un admin
			if (_admins.size() > 1) {
				_ownerFd = _admins.begin()->first;
				_admins.erase(_admins.begin());
				_admins.insert(std::make_pair(_ownerFd, _users[_ownerFd]));
			}
			else
				_ownerFd = -1;
		}
		else
			_admins.erase(client->getFd());
	}
	if (!_ownerFd && !_users.empty()) {
		// Si no hay propietario ni admins pero hay usuarios, se asigna el primero como propietario
		_ownerFd = _users.begin()->first;
		_admins.insert(std::make_pair(_ownerFd, _users[_ownerFd]));
	}
	_users.erase(client->getFd());
}
