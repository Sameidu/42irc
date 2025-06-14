#include <Server.hpp>

void	Server::initCmds()
{
	_fCommands.insert(std::pair<std::string, FCmd>("PASS", &Server::CmPass));

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

void Server::handleCommand(t_msg& msg, int client)
{
	int status;

	/* TODO: mapa std::map< std::string, *func(t_msg, int) */
	if (msg.command == "PASS")
		status = CmPass(msg, client);
	else if (msg.command == "NICK")
		status = CmNick(msg, client);
	else if (msg.command == "USER")
		status = CmUser(msg, client);
	
	answerCLient(status, msg, client);
}

int Server::CmNick(t_msg& msg, int client)
{
	if (arg.size() > MAX_CHAR_NICKNAME)
	{
		if (send(fdClient, "Too long Nickname, max 9 chars\n", 32 , MSG_EOR) < 0)
			throw std::runtime_error("Error: sending msg to client");
		return false;
	}
	/* TODO: mirar si hay otro nickname igual*/
    _clients[fdClient]->setNickname(args);
	return true;
}

int Server::CmUser(t_msg& msg, int fd)
{

}

int Server::CmPass(t_msg& msg, int fd)
{

}


bool	Server::isCorrectNickname(std::string arg, int fdClient)
{
	if (arg.size() > MAX_CHAR_NICKNAME)
	{
		if (send(fdClient, "Too long Nickname, max 9 chars\n", 32 , MSG_EOR) < 0)
			throw std::runtime_error("Error: sending msg to client");
		return false;
	}
	/* TODO: mirar si hay otro nickname igual*/
	return true;
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
