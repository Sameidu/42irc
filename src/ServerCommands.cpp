#include <Server.hpp>
#include <iostream>
#include <sstream>
#include <string>

void	Server::initCmds()
{
	_fCommands.insert(std::pair<std::string, FCmd>("PASS", &Server::CmPass));
	_fCommands.insert(std::pair<std::string, FCmd>("NICK", &Server::CmNick));
	_fCommands.insert(std::pair<std::string, FCmd>("USER", &Server::CmUser));
}

void	Server::answerCLient(int fdClient, int code, std::string msg)
{
	std::stringstream	ss;
	ss << code;
	std::string codeS = ss.str();

	std::string msgBuilt = (":ircserver.com" + codeS + _clients[fdClient]->getNickname() + msg + "\r\n");

	if (send(fdClient, msgBuilt.c_str(), msgBuilt.size() , MSG_EOR) < 0)
			throw std::runtime_error("Error: sending msg to client");
}

void Server::handleCommand(t_msg& msg, int fdClient)
{
	/*if (_clients[fdClient]->getIsConnect() != 3)
	{
		CmPass(msg, fdClient);
		CmNick(msg, fdClient);
		CmUser(msg, fdClient);
		if (_clients[fdClient]->getIsConnect() == 3)
			answerCLient(fdClient, RPL_WELCOME, _clients[fdClient]->getNickname() + " :Welcome to the IRC network, angela");
		else
		{
			// disconnectClient(fdClient);
			return ;
		}
	}*/

	std::map<std::string, FCmd>::iterator it = _fCommands.find(msg.command);

	if (it != _fCommands.end()) 
	{
		std::cout << it->first << std::endl;
		FCmd func = it->second;
		(this->*func)(msg, fdClient);
	} 
	else
		answerCLient(fdClient, ERR_UNKNOWNCOMMAND, _clients[fdClient]->getNickname() + " :Unknown command");

}

/* TODO
nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
letter     =  A-Z a-z
digit      =  0-9
special    =  [ ] \ ` _ ^ { | }
isValidNickname(string)
sendError(int fd, int code, std::string msg)
sendError(fdClient, 433, nick + " :Nickname is already in use");
*/

bool isLetter(char c) 
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(char c) 
{
    return (c >= '0' && c <= '9');
}

bool isSpecial(char c) 
{
    return (c == '[' || c == ']' || c == '\\' || c == '`' ||
            c == '_' || c == '^' || c == '{' || c == '|' );
}

/* TODO: msg error esta mal */

void Server::CmNick(t_msg& msg, int fdClient)
{
	if (msg.params.size() != 1)
		answerCLient(fdClient, ERR_NONICKNAMEGIVEN, _clients[fdClient]->getNickname() + " :Nickname is already in use");	
	/*TODO: msg error 2 many params o incorrect nickname ?? que hace irc */

	if (msg.params[0].size() > MAX_CHAR_NICKNAME)
		answerCLient(fdClient, ERR_ERRONEUSNICKNAME, _clients[fdClient]->getNickname() + " :Nickname is already in use");
		 // TODO: error de demasiado alrgo el nickname

	if (!isSpecial(msg.params[0][0]) && !isLetter(msg.params[0][0]))
		answerCLient(fdClient, ERR_ERRONEUSNICKNAME, _clients[fdClient]->getNickname() + " :Nickname is already in use");	
		 // invalid chars in nickname
	for (std::size_t i = 1; i < msg.params[0].size(); i++)
	{
		char c = msg.params[0][i];
		if (!isLetter(c) && !isSpecial(c) && !isDigit(c) && c != '-')
			answerCLient(fdClient, ERR_ERRONEUSNICKNAME, _clients[fdClient]->getNickname() + " :Nickname is already in use");	
	}
	
	std::map<int, Client*>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first == fdClient)
			continue ;
		else if (it->second->getNickname() == msg.params[0])
			answerCLient(fdClient, ERR_NICKNAMEINUSE, _clients[fdClient]->getNickname() + " :Password incorrect");	
			// TODO: erorr same nick
	}

	if (_clients[fdClient]->getIsConnect() == 1)
		_clients[fdClient]->setIsConnect(_clients[fdClient]->getIsConnect() + 1);
    _clients[fdClient]->setNickname(msg.params[0]);
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
		answerCLient(fdClient, ERR_PASSWDMISMATCH, _clients[fdClient]->getNickname() + " :Password incorrect");	
		std::cout << RED << "contraseña incorrecta" << CLEAR << std::endl;
	}
	else
		_clients[fdClient]->setIsConnect(1);
	/* TODO: No he verificado todo aun solo contraseña*/
}

