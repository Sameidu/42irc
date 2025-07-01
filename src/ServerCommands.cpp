#include <Server.hpp>
#include <iostream>
#include <sstream>
#include <string>

// TODO: Añadir y tener en cuenta los modos 's' y 'p' (secreto y privado) de los canales

void	Server::initCmds()
{
	_fCommands.insert(std::pair<std::string, FCmd>("PASS", &Server::CmPass));
	_fCommands.insert(std::pair<std::string, FCmd>("NICK", &Server::CmNick));
	_fCommands.insert(std::pair<std::string, FCmd>("USER", &Server::CmUser));
	_fCommands.insert(std::pair<std::string, FCmd>("CAP", &Server::CmCAP));
	_fCommands.insert(std::pair<std::string, FCmd>("JOIN", &Server::CmJoin));
	_fCommands.insert(std::pair<std::string, FCmd>("LIST", &Server::CmList));
	_fCommands.insert(std::pair<std::string, FCmd>("PART", &Server::CmPart));
	_fCommands.insert(std::pair<std::string, FCmd>("NAMES", &Server::CmNames));
	_fCommands.insert(std::pair<std::string, FCmd>("TOPIC", &Server::CmTopic)); // Solo para admins
	// _fCommands.insert(std::pair<std::string, FCmd>("KICK", &Server::CmKick)); // Solo para admins
	// _fCommands.insert(std::pair<std::string, FCmd>("INVITE", &Server::CmInvite)); // Solo para admins
	// _fCommands.insert(std::pair<std::string, FCmd>("MODE", &Server::CmMode)); // Solo para admins
	//_fCommands.insert(std::pair<std::string, FCmd>("PRIVMSG", &Server::CmPrivMsg));
	//_fCommands.insert(std::pair<std::string, FCmd>("QUIT", &Server::CmQuit));

}

void Server::answerClient(int fdClient, int code, const std::string &target, const std::string &msg)
{
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(3) << code;  // código en 3 dígitos
	std::string codeStr = ss.str();

	std::string nickname = _clients[fdClient]->getNickname();
	if (nickname.empty())
		nickname = "unknown";
	std::string response = ":ircserver.com " + codeStr + " " + nickname;
	
	if (!target.empty())
		response += " " + target;
	response += " :" + msg + "\r\n";

	std::cout << GREEN << response << CLEAR << std::endl;
	if (send(fdClient, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

/**
 * @brief Envía un mensaje IRC directo a un cliente, sin código numérico.
 *
 * Esta función está pensada para comandos como PRIVMSG o PART, que requieren
 * mensajes directos entre usuarios o notificaciones de eventos, y no respuestas
 * del servidor con códigos numéricos. Por eso, el mensaje se construye siguiendo
 * el formato estándar de IRC con prefijo (nick!user@host), comando, destino y mensaje,
 * pero sin incluir ningún código de respuesta numérico, ya que el cliente no espera
 * ni interpreta códigos en este tipo de mensajes.
 *
 * @param fd        Descriptor del cliente destinatario.
 * @param cmd       Comando IRC (por ejemplo, "PRIVMSG", "PART").
 * @param channel   Canal o destino del mensaje.
 * @param msg       Contenido del mensaje.
 */

void Server::sendMsgToClient(int fd, const std::string &cmd, const std::string &channel, const std::string &msg) {
	std::string prefix = ":" + _clients[fd]->getNickname() + "!" + _clients[fd]->getUsername() + "@localhost";
	std::string response = prefix + " " + cmd + " " + channel;
	
	if (!msg.empty())
		response += " :" + msg;
	response += "\r\n";
	std::cout << BLUE << response << CLEAR << std::endl;
	if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void	Server::sendWelcomeMsg(int fdClient)
{
	/*Client& c = *_clients[fd];
    // 001
    answerClient(fd, RPL_WELCOME, c.getNickname(),
                 "Welcome to the IRC network, " + c.getNickname());
    // 002
    answerClient(fd, RPL_YOURHOST, c.getNickname(),
                 "Your host is " + _servername + ", running version " + _version);
    // 003
    answerClient(fd, RPL_CREATED, c.getNickname(),
                 "This server was created " + _creationDate);
    // 004
    answerClient(fd, RPL_MYINFO, c.getNickname(),
                 _servername + " " + _version + " " + _userModes + " " + _chanModes);
    // 005 
    sendISupport(fd);*/
	answerClient(fdClient, RPL_WELCOME, "", "Welcome to the IRC network, " + _clients[fdClient]->getNickname());
}

void	Server::joinGeneralChannel(int fdClient)
{
	if (_channel.find("#general") == _channel.end()) {
		Channel *newChannel = new Channel("#general");
		_channel.insert(std::pair<std::string, Channel*>("#general", newChannel));
	}
	_channel["#general"]->newChannelUser(_clients[fdClient]);
}

void Server::handleCommand(t_msg& msg, int fdClient)
{
	if (_clients[fdClient]->getRegistrationState() != RS_Registered)
	{
		if (msg.command == "CAP") {
			CmCAP(msg, fdClient);
			return;
		}
		if (_clients[fdClient]->getRegistrationState() == RS_NoPass && msg.command == "PASS")
			CmPass(msg, fdClient);
		else if (_clients[fdClient]->getRegistrationState() == RS_PassValidated && msg.command == "NICK")
			CmNick(msg, fdClient);
		else if (_clients[fdClient]->getRegistrationState() == RS_NickValidated  && msg.command == "USER")
			CmUser(msg, fdClient);

		if (_clients[fdClient]->getRegistrationState() == RS_Registered)
		{
			sendWelcomeMsg(fdClient);
			joinGeneralChannel(fdClient);
		}
		return ;
	}

	std::map<std::string, FCmd>::iterator it = _fCommands.find(msg.command);
	if (it != _fCommands.end()) 
	{
		std::cout << it->first << std::endl;
		FCmd func = it->second;
		(this->*func)(msg, fdClient);
	} 
	else
		answerClient(fdClient, ERR_UNKNOWNCOMMAND, "", _clients[fdClient]->getNickname() + " Unknown command");
}


