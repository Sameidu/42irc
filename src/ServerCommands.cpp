#include <Server.hpp>

void	Server::initCmds()
{
	_fCommands.insert(std::pair<std::string, FCmd>("PASS", &Server::CmPass));
	_fCommands.insert(std::pair<std::string, FCmd>("NICK", &Server::CmNick));
	_fCommands.insert(std::pair<std::string, FCmd>("USER", &Server::CmUser));
}

/* TODO: archivo totalmente inutil por el momento e incompleto */


/*
	Un mensaje IRC puede tener hasta 15 parámetros en total:

	14 "middle" parameters → los que van separados por espacios

	1 "trailing" parameter → que empieza con : → puede contener espacios

	si el param empieza con #canal es un canal si no empieza con nada es el target angela a quien se loq ueires enviar

	los parametros dependen de cada comando si son utiles o necesarios
*/

void	Server::answerCLient(int status, t_msg& msg, int fdClient)
{
	/* TODO: dependiendo el codigo de respuesta crear un msg diferente MAPA?*/
	std::string msgToClient = "hola";

	std::string uwu = (":ircserver.com" + status + _clients[fdClient]->getNickname() + ":" + msgToClient + "\r\n");

	if (send(fdClient, &uwu, sizeof(uwu) , MSG_EOR) < 0)
			throw std::runtime_error("Error: sending msg to client");
}

void Server::handleCommand(t_msg& msg, int fdClient)
{
	int status;

	/* TODO: mapa std::map< std::string, *func(t_msg, int) */
	if (msg.command == "PASS")
		status = CmPass(msg, fdClient);
	else if (msg.command == "NICK")
		status = CmNick(msg, fdClient);
	else if (msg.command == "USER")
		status = CmUser(msg, fdClient);
	
	answerCLient(status, msg, fdClient);
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

int Server::CmNick(t_msg& msg, int fdClient)
{
	/* TODO: me falta validad caracteres validos para el nick */

	if (msg.params.size() != 1)
		return -1; /*TODO: msg error 2 many params o incorrect nickname ?? que hace irc */
	if (msg.params[0].size() > MAX_CHAR_NICKNAME)
		return -1; // TODO: error de demasiado alrgo el nickname
	
	std::map<int, Client*>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first == fdClient)
			continue ;
		else if (it->second->getNickname() == msg.params[0])
			return -1; // TODO: erorr same nick
	}

    _clients[fdClient]->setNickname(msg.params[0]);

	return 1;
}

int Server::CmUser(t_msg& msg, int fd)
{

}

int Server::CmPass(t_msg& msg, int fd)
{

}


void	Server::createUserForClient(std::string args, std::string command, int fdClient)
{
	/* el cliente tiene que mandar exactamente estos comando en orden PASS, NICK, USER */
	if (_clients[fdClient]->getIsConnect() == 0 && command == "PASS")
	{
		/* Solo tiene 3 intentos */
		if ( args == _password )
		{
			_clients[fdClient]->setIsConnect(1);
			if (send(fdClient, "Correct password, now enter your unique nickname\n", 50 , MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
		}
		else
		{
			if (send(fdClient, "Incorrect password, try again\n", 31 , MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
			if (_clients[fdClient]->getTimesWrongPass() < MAX_PASS_TRY)
			{
				// TODO: cerrar el cliente??
				std::cout << "DEBERIA DE PARAR" << std::endl;				}
			else
				_clients[fdClient]->setTimesWrongPass(_clients[fdClient]->getTimesWrongPass() + 1);
		}
	}
	else if (_clients[fdClient]->getIsConnect() == 1 && command == "NICK")
	{
		if (isCorrectNickname(args, fdClient))
		{
			_clients[fdClient]->setIsConnect(2);
			_clients[fdClient]->setNickname(args);
			if (send(fdClient, "Nick OK, now enter your user\n", 30 , MSG_EOR) < 0)
				throw std::runtime_error("Error: sending msg to client");
		}
	}
	else if (_clients[fdClient]->getIsConnect() == 2 && command == "USER")
	{
		_clients[fdClient]->setIsConnect(3);
		_clients[fdClient]->setUsername(args);
		if (send(fdClient, "You are connected to the server\n", 33 , MSG_EOR) < 0)
			throw std::runtime_error("Error: sending msg to client");
		/* TODO: Upon success, the client will receive an RPL_WELCOME (for users)*/
	}
	else
		std::cout << "you are not connected to the server" << std::endl;
		/* TODO: mandar msg al cliente?*/
}
