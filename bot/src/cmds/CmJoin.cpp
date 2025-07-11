#include <Bot.hpp>

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void Bot::CmJoin(t_msg &msg, int fd) {
	(void)msg;
    (void)fd;
	// /* 1.- Comprobar que los parametros sean como mucho 2
	//  * 2.- Parsear los parametros y separarlos por "," por si se quisiera unir a varios canales a la vez
	//  * 3.- Enlazar canal con constraseña si lo tuvienra segun parametros
	//  * 4.- Por cada par creado, comprobar que el canal existe y unir al cleinte si reune las condiciones 
	//  *  (En caso de que esté el modo invitacion activado comprobar que está en esta lista) (Comprobar si está banneado) (Constraseña y maxusers)
	//  * 5.- Si no existe el canal, crearlo y añadir al cliente
	//  * 6.- Si todo ok, añadir al cliente al canal y enviar mensaje de bienvenida a este.
	//  */
	// if (msg.params.size() < 1 || msg.params.size() > 2) {
	// 	answerClient(fd, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
	// 	return ;
	// }

	// std::vector<std::string> channels;
	// std::vector<std::string> passwords;

	// splitCmd(msg.params[0], channels, ',');
	// if (msg.params.size() == 2)
	// 	splitCmd(msg.params[1], passwords, ',');

	// // TODO: para el caso de 0 solo debe haber 1 parametro
	// for (size_t i = 0; i < channels.size(); ++i) {
	// 	if (channels[i][0] != '#' && channels[i][0] != '&' && channels[i] != "0") {
	// 		answerClient(fd, ERR_BADCHANMASK, "", "Channel name must start with #, & or 0");
	// 		return ;
	// 	}
	// 	if (channels[i] == "0") {
	// 		if (channels.size() > 1 || msg.params.size() > 1) {
	// 			answerClient(fd, ERR_NEEDMOREPARAMS, "JOIN", "Channel '0' must be used alone");
	// 			return ;
	// 		}
	// 		std::string partChannels;
	// 		for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
	// 			if (it->second->hasUser(fd)) {
	// 				if (!partChannels.empty())
	// 					partChannels += ",";
	// 				partChannels += it->first;
	// 			}
	// 		}
	// 		if (!partChannels.empty()) {
	// 			t_msg partMsg;
	// 			partMsg.command = "PART";
	// 			partMsg.params.push_back(partChannels);
	// 			CmPart(partMsg, fd);
	// 		}
	// 	}
	// 	else {
	// 		// Si no existe el canal crear uno nuevo, si pasan contraseña activar el modo k
	// 		if (_channel.find(channels[i]) == _channel.end()) {
	// 			Channel *newChannel = new Channel(channels[i]);
	// 			if (passwords.size() > i && !passwords[i].empty()) {
	// 				newChannel->setPass(passwords[i]);
	// 				newChannel->setMode('k');
	// 			}
	// 			_channel.insert(std::pair<std::string, Channel*>(channels[i], newChannel));
	// 		}
	// 		// Comprobar que el cliente no forme ya parte del canal
	// 		if (_channel[channels[i]]->hasUser(fd)) {
	// 			answerClient(fd, ERR_ALREADYONCHANNEL, channels[i], "You are already in the channel");
	// 			continue ;
	// 		}
	// 		// Comprobar que el canal no está lleno y puede unirse
	// 		if (_channel[channels[i]]->hasMode('l') && _channel[channels[i]]->getUserCount() >= _channel[channels[i]]->getMaxUsers()) {
	// 			answerClient(fd, ERR_CHANNELISFULL, channels[i], "Channel is full (+l)");
	// 			continue ;
	// 		}
	// 		// Comprobar si el canal requiere contraseña y esta bien
	// 		if (_channel[channels[i]]->hasMode('k')) {
	// 			if (passwords.size() <= i || passwords[i] != _channel[channels[i]]->getPass()) {
	// 				answerClient(fd, ERR_BADCHANNELKEY, channels[i], "Cannot join channel (+k)");
	// 				continue ;
	// 			}
	// 		}
	// 		// Comprobar si el canal esta en modo invitacion y el usuario se encuentra en la lista
	// 		if (_channel[channels[i]]->hasMode('i')) {
	// 			if (!_channel[channels[i]]->isInvited(fd)) {
	// 				answerClient(fd, ERR_INVITEONLYCHAN, channels[i], "Cannot join channel (+i)");
	// 				continue ;
	// 			}
	// 			else
	// 				_channel[channels[i]]->removeInvitedList(_clients[fd]);
	// 		}
	// 		// Comprobar si el canal tiene lista de banneados y el usuario se encuentra en ella
	// 		if (_channel[channels[i]]->hasMode('b')) {
	// 			if (_channel[channels[i]]->isBanned(fd)) {
	// 				answerClient(fd, ERR_BANNEDFROMCHAN, channels[i], "Cannot join channel (+b)");
	// 				continue ;
	// 			}
	// 		}
	// 		// Si todo ok, añadir al cliente al canal
	// 		_channel[channels[i]]->newChannelUser(_clients[fd]);
	// 		// Se indica a los clientes que alguien se ha unido, pero no se le añade mensaje personalizado
	// 		_channel[channels[i]]->broadcastMessage(fd, "JOIN", "", "");
	// 		sendMsgToClient(fd, "JOIN", channels[i], "");
	// 		// Se le pasa el TOPIC al nuevo cliente si existe
	// 		t_msg topic;
	// 		topic.command = "TOPIC";
	// 		topic.params.push_back(channels[i]);
	// 		topic.trailing = msg.trailing;
	// 		CmTopic(topic, fd);
	// 		// Se le pasa la lista de usuarios del canal conectado
	// 		t_msg names;
	// 		names.command = "NAMES";
	// 		names.params.push_back(channels[i]);
	// 		CmNames(names, fd);
	// 	}
	// }
}