#include <Server.hpp>

void	Server::initCmds()
{
	_fCommands.insert(std::pair<std::string, FCmd>("PASS", &Server::CmPass));
	_fCommands.insert(std::pair<std::string, FCmd>("NICK", &Server::CmNick));
	_fCommands.insert(std::pair<std::string, FCmd>("USER", &Server::CmUser));
	_fCommands.insert(std::pair<std::string, FCmd>("CAP", &Server::CmCAP));
	//_fCommands.insert(std::pair<std::string, FCmd>("JOIN", &Server::CmJoin));
	_fCommands.insert(std::pair<std::string, FCmd>("LIST", &Server::CmList));
	//_fCommands.insert(std::pair<std::string, FCmd>("PRIVMSG", &Server::CmPrivMsg));
}

void Server::sendMsgToClient(int fd, const std::string &cmd, const std::string &channel, const std::string &msg) {
	std::string prefix = ":" + _clients[fd]->getNickname() + "!" + _clients[fd]->getUsername() + "@localhost";
	std::string response = prefix + " " + cmd + " " + channel;
	
	if (!msg.empty())
		response += " :" + msg;
	response += "\r\n";
	if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void Server::answerCLient(int fdClient, int code, const std::string& msg)
{
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(3) << code;  // código en 3 dígitos
	std::string codeStr = ss.str();

	std::string nickname = _clients[fdClient]->getNickname();
	if (nickname.empty())
		nickname = "unknown";
	std::string response = ":ircserver.com " + codeStr + " " + nickname + " :" + msg + "\r\n";

	std::cout << GREEN << response << CLEAR << std::endl;
	if (send(fdClient, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void Server::handleCommand(t_msg& msg, int fdClient)
{
	if (_clients[fdClient]->getIsConnect() != 3)
	{
		if (msg.command == "CAP") {
			CmCAP(msg, fdClient);
			return;
		}
		if (_clients[fdClient]->getIsConnect() == 0 && msg.command == "PASS")
			CmPass(msg, fdClient);
		else if (_clients[fdClient]->getIsConnect() == 1 && msg.command == "NICK")
			CmNick(msg, fdClient);
		else if (_clients[fdClient]->getIsConnect() == 2  && msg.command == "USER")
			CmUser(msg, fdClient);
		else
			return ;

		if (_clients[fdClient]->getIsConnect() == 3) {
			if (_channel.find("#general") == _channel.end()) {
				Channel *newChannel = new Channel("#general");
				_channel.insert(std::pair<std::string, Channel*>("#general", newChannel));
			}
			_channel["#general"]->newChannelUser(_clients[fdClient]);
			answerCLient(fdClient, RPL_WELCOME, "Welcome to the IRC network, angela");
		}
	}

	std::map<std::string, FCmd>::iterator it = _fCommands.find(msg.command);
	// TODO: Si el cliente no está validado no se debe ejecutar ningún comando
	if (it != _fCommands.end()) 
	{
		std::cout << it->first << std::endl;
		FCmd func = it->second;
		(this->*func)(msg, fdClient);
	} 
	else
		answerCLient(fdClient, ERR_UNKNOWNCOMMAND, _clients[fdClient]->getNickname() + " :Unknown command");

}

void Server::CmNick(t_msg& msg, int fdClient)
{
	if (msg.params.empty() && msg.trailing.size() == 0)
		answerCLient(fdClient, ERR_NONICKNAMEGIVEN, "No nickname given"); // ok, no manda nada
	else if (msg.params.empty() && msg.trailing.size() != 0)
		msg.params[0] = msg.trailing;

	if (msg.params[0].size() > MAX_CHAR_NICKNAME)
		answerCLient(fdClient, ERR_ERRONEUSNICKNAME, "Erroneus nickname"); // ok

	if (!isSpecial(msg.params[0][0]) && !isalpha(msg.params[0][0]))
		answerCLient(fdClient, ERR_ERRONEUSNICKNAME, "Erroneus nickname");	// ok

	for (std::size_t i = 1; i < msg.params[0].size(); i++)
	{
		char c = msg.params[0][i];
		if (!isalpha(c) && !isSpecial(c) && !isdigit(c) && c != '-')
			answerCLient(fdClient, ERR_ERRONEUSNICKNAME, "Erroneus nickname");	// ok
	}
	
	std::map<int, Client*>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first == fdClient)
			continue ;
		else if (it->second->getNickname() == msg.params[0])
			answerCLient(fdClient, ERR_NICKNAMEINUSE, "Nickname is already in use"); // ok
	}

	if (_clients[fdClient]->getIsConnect() == 1)
		_clients[fdClient]->setIsConnect(_clients[fdClient]->getIsConnect() + 1);
    _clients[fdClient]->setNickname(msg.params[0]);
	/* TODO: cuando se cambia el nick hay que mandar un msg de confirmacion al cliente y un msg,
		de que se cambio su nick a todos los canales en los que esta*/
}

void	Server::CmUser(t_msg& msg, int fdClient)
{
	if (_clients[fdClient]->getIsConnect() == 2)
		_clients[fdClient]->setIsConnect(_clients[fdClient]->getIsConnect() + 1);
	_clients[fdClient]->setUsername(msg.params[0]);
}

void	Server::CmPass(t_msg& msg, int fdClient)
{
	std::cout << "----------->>>>>>" << _password << std::endl;
	std::cout << "----------->>>>>>" << msg.params[0] << std::endl;
	if (_password != msg.params[0])
	{
		answerCLient(fdClient, ERR_PASSWDMISMATCH, "Password incorrect");
		std::cout << RED << "contraseña incorrecta" << CLEAR << std::endl;
	}
	else
		_clients[fdClient]->setIsConnect(1);
	/* TODO: No he verificado todo aun solo contraseña*/
}

void Server::CmCAP(t_msg& msg, int fd)
{
	if (msg.params.size() < 2 || msg.params[0] != "LS")
	{
		answerCLient(fd, ERR_UNKNOWNCOMMAND, "Unknown command");
		return ;
	}
	answerCLient(fd, 302, "multi-prefix");
}

void Server::CmList(t_msg &msg, int fd) {
	if (msg.params.size() > 1) {
		answerCLient(fd, ERR_UNKNOWNCOMMAND,"Too many parameters");
		return;
	}
	if (_channel.empty()) {
		answerCLient(fd, RPL_LISTEND, "End of /LIST");
		return;
	}
	answerCLient(fd, RPL_LISTSTART, "Channel  Users  Name");
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		std::string response = it->first + " " + to_string(it->second->getUserCount()) + " " + it->second->getName();
		answerCLient(fd, RPL_LISTITEM, response);
	}
	answerCLient(fd, RPL_LISTEND, "End of /LIST");
}
