#include <Server.hpp>

// PART <channel>{,<channel>} [<reason>]
void Server::CmPart(t_msg &msg, int fd) {
	/* 1.- Comprobar que los parametros sean como mucho 2
	 * 2.- Parsear los parametros y separarlos por "," por si se quisiera salir de varios canales a la vez
	 * 3.- Por cada canal, comprobar que el cliente está en él
	 * 4.- Si no está, enviar error de no estar en el canal
	 * 5.- Si está, eliminar al cliente del canal y enviar mensaje de despedida
	 * La razón de abandono puede pasarla el cliente, sino se enviará una genérica
	 * 6.- Si el canal se queda vacío, eliminarlo del servidor
	 */
	if (msg.params.size() < 1 || msg.params.size() > 2) {
		answerClient(fd, ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
		return ;
	}

	std::vector<std::string> channels;
	splitCmd(msg.params[0], channels, ',');

	if (!channels.empty()) {
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
			if (_channel.find(*it) == _channel.end()) {
				answerClient(fd, ERR_NOSUCHCHANNEL, "", "No such channel " + *it);
				continue ;
			}
			if (!_channel[*it]->hasUser(fd)) {
				answerClient(fd, ERR_NOTONCHANNEL, "", "You're not on that channel " + *it);
				continue ;
			}
			std::string reason = (!msg.trailing.empty()) ? msg.trailing : _clients[fd]->getNickname() + " has left " + *it;
			sendMsgToClient(fd, "PART", *it, "Leaving... ");
			_channel[*it]->broadcastMessage(fd, "PART", "", reason);
			_channel[*it]->disconnectUser(_clients[fd]);
			if (_channel[*it]->getUserCount() == 0) {
				delete _channel[*it];
				_channel.erase(*it);
			}
		}
	}
}