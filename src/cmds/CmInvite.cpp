#include <Server.hpp>

// INVITE <nickname> <channel>
void Server::CmInvite(t_msg &msg, int fd) {
	// Esto solo lo pueden hacer los admins si lo intenta un usuario normal dar error
	/* 1.- Comprobar que los parametros sean 2
	 * 2.- Comprobar que el cliente al que se invita exista
	 * 3.- Comprobar que el canal al que se invita exista
	 * 4.- Comprobar que el cliente que invita no esté en el canal ni en la lista de invitados
	 * 5.- Comprobar que quien invita sea admin del canal si esta activo el modo invite-only
	 * 6.- Añadir al cliente invitado a la lista de invitados del canal
	 * 7.- Enviar mensaje al cliente invitado de que ha sido invitado al canal
	 */
	if (msg.params.size() != 2) {
		answerClient(fd, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
		return ;
	}

	int invited = -1;
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickname() == msg.params[0]) {
			invited = it->first;
			break;
		}
	}
	if (invited == -1) {
		answerClient(fd, ERR_NOSUCHNICK, msg.params[0], "No such nick/channel");
		return ;
	}
	if (_channel.find(msg.params[1]) == _channel.end()) {
		answerClient(fd, ERR_NOTONCHANNEL, msg.params[1], "You're not on that channel");
		return ;
	}
	if (!_channel[msg.params[1]]->hasUser(_clients[fd]->getNickname())) {
		answerClient(fd, ERR_NOTONCHANNEL, msg.params[1], "You're not on that channel");
		return ;
	}
	if (_channel[msg.params[1]]->hasUser(msg.params[0])) {
		answerClient(fd, ERR_USERONCHANNEL, msg.params[0], "is already on channel");
		return ;
	}
	if (_channel[msg.params[1]]->hasMode('i') && !_channel[msg.params[1]]->isAdmin(fd)) {
		answerClient(fd, ERR_INVITEONLYCHAN, msg.params[1], "Cannot invite to invite-only channel");
		return ;
	}
	if (_channel[msg.params[1]]->hasMode('l') && _channel[msg.params[1]]->getUserCount() >= _channel[msg.params[1]]->getMaxUsers()) {
		answerClient(fd, ERR_CHANNELISFULL, msg.params[1], "Cannot invite to full channel");
		return ;
	}
	if (_channel[msg.params[1]]->hasMode('b') && _channel[msg.params[1]]->isBanned(invited)) {
		answerClient(fd, ERR_BANNEDFROMCHAN, msg.params[1], "Cannot invite banned user");
		return ;
	}
	if (_channel[msg.params[1]]->isInvited(invited)) {
		answerClient(fd, ERR_USERONCHANNEL, msg.params[0], "is already invited to channel");
		return ;
	}
	_channel[msg.params[1]]->addInvitedList(_clients[invited]);
	msgClientToClient(fd, invited, "INVITE", msg.params[1]);
	answerClient(fd, RPL_INVITING, _clients[invited]->getNickname(), msg.params[1]);
}
