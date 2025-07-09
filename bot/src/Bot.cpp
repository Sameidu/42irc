# include <Bot.hpp>

Bot::Bot(const std::string &ip, const int &port, const std::string &password)
	: _fd(-1), _password(password), _nick("Bot"), _user("Bot"), _name("Bot"), _serverIp(ip), _serverPort(port) {
		if (ip == "localhost")
			_serverIp = "127.0.0.1";
	}

Bot::~Bot() {}

void Bot::start() {
	connectToServer();
	sendCredentials();
	eventListen();
}

void Bot::connectToServer() {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) 
		throw std::runtime_error("Error: creating the socket for the bot");
	std::cout << "Bot socket created with fd -> " << _fd << std::endl;

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Error: when setting SO_REUSEADDR");
	
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: setting flags to non blocking for bot socket");

	sockaddr_in serv;
	std::memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(_serverIp.c_str());
	serv.sin_port = htons(_serverPort);

	if (connect(_fd, (sockaddr*)&serv, sizeof(serv)) < 0) {
		if (errno == EINPROGRESS)
			std::cout << "Connection in progress..." << std::endl;
		else
			throw std::runtime_error("Error: connecting to the server");
	}
}

void Bot::sendCredentials() {
	std::string passCmd = "PASS " + _password + "\r\n";
	std::string nickCmd = "NICK " + _nick + "\r\n";
	std::string userCmd = "USER " + _user + " 0 * :" + _name + "\r\n";

	if (send(_fd, passCmd.c_str(), passCmd.size(), 0) < 0)
		throw std::runtime_error("Error: sending PASS command");
	if (send(_fd, nickCmd.c_str(), nickCmd.size(), 0) < 0)
		throw std::runtime_error("Error: sending NICK command");
	if (send(_fd, userCmd.c_str(), userCmd.size(), 0) < 0)
		throw std::runtime_error("Error: sending USER command");
}

void Bot::eventListen() {
	char buffer[MAX_BYTES_MSG];
	while (true) {
		std::memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			else
				throw std::runtime_error("Error: receiving data from server");
		} else if (bytesReceived == 0) {
			std::cout << "Server closed the connection." << std::endl;
			break;
		}
		buffer[bytesReceived] = '\0';
		std::cout << "Received: " << buffer << std::endl;
	}
}