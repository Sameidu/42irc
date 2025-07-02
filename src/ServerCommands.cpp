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
	_fCommands.insert(std::pair<std::string, FCmd>("KICK", &Server::CmKick)); // Solo para admins
	_fCommands.insert(std::pair<std::string, FCmd>("INVITE", &Server::CmInvite)); // Solo para admins
	_fCommands.insert(std::pair<std::string, FCmd>("MODE", &Server::CmMode)); // Solo para admins
	//_fCommands.insert(std::pair<std::string, FCmd>("PRIVMSG", &Server::CmPrivMsg));
	//_fCommands.insert(std::pair<std::string, FCmd>("QUIT", &Server::CmQuit));

}

// Ahora si envia la resuesta bien al cliente
/* No poner en los mensajes enviados cuando se llame a esta función ni el nick
 ni los ":" porque sino  el cliente no lo interpreta bien 
 Solo el mensaje que se desee enviar (Como mucho un espacio al principio) */

void Server::answerClient(int fdClient, int code, const std::string &target, const std::string &msg)
{
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(3) << code;  // código en 3 dígitos
	std::string codeStr = ss.str();

	std::string nickname = _clients[fdClient]->getNickname();
	if (nickname.empty())
		nickname = "unknown";
	std::string response = ":localhost " + codeStr + " " + nickname;
	
	if (!target.empty())
		response += " " + target;
	response += " :" + msg + "\r\n";

	std::cout << GREEN << response << CLEAR << std::endl;
	if (send(fdClient, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void Server::answerClient(int fdClient, int code, char &target, const std::string &msg)
{
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(3) << code;  // código en 3 dígitos
	std::string codeStr = ss.str();

	std::string nickname = _clients[fdClient]->getNickname();
	if (nickname.empty())
		nickname = "unknown";
	std::string response = ":localhost " + codeStr + " " + nickname;

	if (target)
		response += " " + target;
	response += " :" + msg + "\r\n";

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
	if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void Server::msgClientToClient(int from, int to, const std::string &cmd, const std::string &msg) {
	std::string prefix = ":" + _clients[from]->getNickname() + "!" + _clients[from]->getUsername() + "@localhost";
	std::string response = prefix + " " + cmd + " " + _clients[to]->getNickname();
	
	if (!msg.empty())
		response += " :" + msg;
	response += "\r\n";
	if (send(to, response.c_str(), response.size(), MSG_EOR) < 0)
		throw std::runtime_error("Error: sending msg to client");
}

void Server::handleCommand(t_msg& msg, int fdClient)
{
	if (_clients[fdClient]->getIsConnect() != 3)
	{
		if (msg.command == "CAP") {
			CmCAP(msg, fdClient);
			return ;
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
			t_msg join;
			join.command = "JOIN";
			join.params.push_back("#general");
			CmJoin(join, fdClient);
			answerClient(fdClient, RPL_WELCOME, "", "Welcome to the IRC network, " + _clients[fdClient]->getNickname() + "!");
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
		answerClient(fdClient, ERR_UNKNOWNCOMMAND, "", _clients[fdClient]->getNickname() + " Unknown command");

}


