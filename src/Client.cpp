#include <Client.hpp>

Client::~Client() {}

//Client::Client(const int fd) : _clientFd(fd){}
Client::Client(const int fd, sockaddr_in *clientAddr) : _clientFd(fd), _clientAddr(clientAddr), _isConnect(0), _timesWrongPass(0) {
    (void)_clientAddr;
}

// GETTERS

const std::string &Client::getUsername() const { return _userName; }

const std::string &Client::getNickname() const { return _nickName; }

const std::string &Client::getRealname() const { return _realName; }

const int &Client::getIsConnect() const { return _isConnect;}

const int &Client::getTimesWrongPass() const { return _timesWrongPass;}


int Client::getFd() const { return _clientFd; }

// SETTERS

void Client::setUsername(const std::string &username) { _userName = username; }

void Client::setNickname(const std::string &nickname) { _nickName = nickname; }

void Client::setRealname(const std::string &realname) { _realName = realname; }

void Client::setIsConnect(const int &isConnect) { _isConnect = isConnect; }

void Client::setTimesWrongPass(const int &timesWrongPass) {  _timesWrongPass =  timesWrongPass; }
