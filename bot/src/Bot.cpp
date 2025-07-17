# include <Bot.hpp>

Bot::Bot(const std::string &ip, const int &port, const std::string &password)
	: _fd(-1), _running(true), _password(password), _nick("Bot"), _user("Bot"), _name("Bot"), _serverIp(ip), _serverPort(port) {
		if (ip == "localhost")
			_serverIp = "127.0.0.1";
	}

Bot::~Bot() {
	if (_fd != -1) {
		close(_fd);
		std::cout << "Bot socket closed." << std::endl;
	}
	std::cout << "Bot destroyed." << std::endl;
}

void Bot::start() {
	connectToServer();
	initCmds();
	initBotCmds();
	sendCredentials();
	while (_running) {
		readMsg();
	}
}

void Bot::initBotCmds() {
	_botCmds.insert(std::pair<std::string, botCmd>("!help", &Bot::helpMsg));
	_botCmds.insert(std::pair<std::string, botCmd>("!wisdom", &Bot::enlightenMe));
	_botCmds.insert(std::pair<std::string, botCmd>("!dice", &Bot::rollDice));
	_botCmds.insert(std::pair<std::string, botCmd>("!rps", &Bot::playRPS));
	_botCmds.insert(std::pair<std::string, botCmd>("!nones", &Bot::oddEven));
	_botCmds.insert(std::pair<std::string, botCmd>("!8ball", &Bot::magic8Ball));
	_botCmds.insert(std::pair<std::string, botCmd>("!say", &Bot::saySomething));
}

void Bot::initCmds() {
	_cmds.insert(std::pair<std::string, FCmd>("INVITE", &Bot::CmInvite));
	_cmds.insert(std::pair<std::string, FCmd>("PRIVMSG", &Bot::CmPrivMsg));
	_cmds.insert(std::pair<std::string, FCmd>("JOIN", &Bot::CmJoin));
}

void Bot::connectToServer() {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) 
		throw std::runtime_error("Error: creating the socket for the bot");
	std::cout << "Bot socket created with fd -> " << _fd << std::endl;
	sckt = _fd;

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

void Bot::readMsg() {
	char msg[MAX_BYTES_MSG];

	std::memset(msg, 0, sizeof(msg));
	int bytesReceived = recv(_fd, msg, sizeof(msg) - 1, 0);

	if (bytesReceived < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		else
			throw std::runtime_error("Error: receiving data from server");
	}
	else if (bytesReceived == 0) {
		std::cout << "Server closed the connection." << std::endl;
		_running = false;
	}

	std::string aux = _bufferMsg;
	aux.append(msg, bytesReceived);
	_bufferMsg = aux;

	std::string& buffer = _bufferMsg;
	std::size_t pos;
	while ((pos = buffer.find("\n")) != std::string::npos) {
		std::string fullMsg = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);

		t_msg parsedMsg = parseMsg(fullMsg);
		handleCommand(parsedMsg);
	}
}

t_msg	Bot::parseMsg(std::string fullMsg)
{
	while (!fullMsg.empty() && (fullMsg[fullMsg.size() - 1] == '\r' || fullMsg[fullMsg.size() - 1] == '\n'))
    	fullMsg.resize(fullMsg.size() - 1);

	t_msg	msg;
	msg.allMsg = fullMsg;
	msg.hasTrailing = false;

	size_t firstChar = fullMsg.find_first_not_of(' ');
	if (firstChar != std::string::npos)
    	fullMsg = fullMsg.substr(firstChar);

	size_t firstSpace = fullMsg.find(' ');
	std::string args;

	if (firstSpace == std::string::npos) {
		msg.prefix = fullMsg.substr(1, firstSpace);
		args = "";
	}
	else {
		msg.prefix = fullMsg.substr(1, firstSpace);
		if (msg.prefix.find('!') != std::string::npos)
			msg.from = msg.prefix.substr(0, msg.prefix.find('!'));
		else
			msg.from = "server";
		args = fullMsg.substr(firstSpace + 1);
	}

	std::size_t trailingPos = args.find(':');
	if (trailingPos != std::string::npos) {
		msg.trailing = args.substr(trailingPos + 1);
		msg.hasTrailing = true;
		args = args.substr(0, trailingPos);
	}
	
	while(!args.empty() && msg.params.size() < 13) {
		size_t pos = args.find(' ');
		std::string token;

		if (pos == std::string::npos) {
			token = args;
			args.clear();
		}
		else {
			token = args.substr(0, pos);
			args = args.substr(pos + 1);
		}

		msg.params.push_back(token);

		size_t nonSpace = args.find_first_not_of(' ');

		if (nonSpace != std::string::npos)
			args = args.substr(nonSpace);
		else
			args.clear();
	}

	if (trailingPos == std::string::npos && !args.empty() && msg.params.size() < 14) {
		size_t pos = args.find(' ');
		if (pos == std::string::npos)
			msg.params.push_back(args);
		else {
			args = args.substr(0, pos);
			msg.params.push_back(args);
		}
	}
	if (msg.from == "server" && !msg.params.empty()) {
		msg.code = strtol(msg.params[0].c_str(), NULL, 10);
		msg.params.erase(msg.params.begin());
	}
	else if (!msg.params.empty()) {
		msg.command = msg.params[0];
		msg.params.erase(msg.params.begin());
	}
	return msg;
}

void Bot::sendMsg(const std::string &type, std::string &to, const std::string &msg) {
	std::string fullMsg = type + " " + to + " :" + msg + "\r\n";
	if (send(_fd, fullMsg.c_str(), fullMsg.size(), MSG_EOR) < 0) {
		std::cerr << "Error sending message to " << to << std::endl;
		return ;
	}
}

void Bot::handleCommand(t_msg &msg) {
	std::cout << PINK <<"Parsed message: " << msg.allMsg << CLEAR << std::endl;
	std::cout << GREEN << "Prefix: " << msg.prefix <<  CLEAR << std::endl;

	std::cout << BLUE << "From: " << msg.from <<  CLEAR << std::endl;
	if (msg.code)
		std::cout << YELLOW << "Code: " << msg.code <<  CLEAR << std::endl;
	if (!msg.command.empty())
		std::cout <<  YELLOW << "Command: " << msg.command << CLEAR << std::endl;

	for (size_t i = 0; i < msg.params.size(); ++i)
		std::cout << RED << "Param[" << i << "]: " << msg.params[i] << CLEAR << std::endl;
	if (msg.hasTrailing)
		std::cout << GREEN << "Trailing: " << msg.trailing <<  CLEAR << std::endl;

	if (msg.from == "server") {
		if (msg.code == 433) {
			std::cerr << RED << "Nickname already in use, please change it." << CLEAR << std::endl;
			_running = false;
		}
		return ;
	}
	else {
		std::map<std::string, FCmd>::iterator it = _cmds.find(msg.command);
		if (it != _cmds.end())
			(this->*(it->second))(msg);
		else 
			std::cout << RED << "Command not recognized: " << msg.command << CLEAR << std::endl;
	}
}