#include <Client.hpp>

Client::~Client() {}

Client::Client(const int fd, sockaddr_in *clientAddr) : _clientFd(fd), _clientAddr(clientAddr), _registrationState(RS_NoPass), _shouldDisconnect(false) {
    _ip = inet_ntoa(_clientAddr->sin_addr);
}

// GETTERS

const std::string &Client::getUsername() const { return _userName; }

const std::string &Client::getNickname() const { return _nickName; }

const std::string &Client::getRealname() const { return _realName; }

const RegistrationStatus &Client::getRegistrationState() const { return _registrationState;}

std::string &Client::getBufferMsgClient() { return _bufferMsgClient; }

std::vector<Channel*>& Client::getChannels() { return _channels; }

int Client::getFd() const { return _clientFd; }

const std::string Client::GetIp() const { return _ip; }

// SETTERS

void Client::setUsername(const std::string &username) { _userName = username; }

void Client::setNickname(const std::string &nickname) { _nickName = nickname; }

void Client::setRealname(const std::string &realname) { _realName = realname; }

void Client::setRegistrationState(RegistrationStatus st) { _registrationState = st; }

void Client::setBufferMsgClient(const std::string &msg) { _bufferMsgClient = msg; }

void Client::joinChannel(Channel* ch) { _channels.push_back(ch); }

void Client::leaveChannel(Channel* ch) {
    _channels.erase(std::remove(_channels.begin(), _channels.end(), ch),_channels.end());
}

void Client::setHasPendingMsg(bool hasPending) { _hasPendingMsg = hasPending; }

bool Client::hasPendingMsg() const { return _hasPendingMsg; }

std::string &Client::getNextMsg() { return _finalMsg; }

void Client::addMsg(const std::string &msg) {
	if (!_finalMsg.empty())
		_finalMsg += "\r\n";
	_finalMsg += msg;
	setHasPendingMsg(true);
}

void Client::updateMsg(size_t sentBytes) {
	if (sentBytes >= _finalMsg.size()) {
		_finalMsg.clear();
		setHasPendingMsg(false);
	} else {
		_finalMsg.erase(0, sentBytes);
	}
}

bool Client::setShouldDisconnect(bool shouldDisconnect) {
	if (shouldDisconnect)
		_shouldDisconnect = shouldDisconnect;
	return _shouldDisconnect;
}

bool Client::getShouldDisconnect() const {
	return _shouldDisconnect;
}