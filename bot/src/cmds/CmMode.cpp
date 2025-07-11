#include <Bot.hpp>

// MODE <channel/user> [<mode> [<mode params>]]
// Con hexchat si no se especifica canal añade automiaticamente el canal en el eque se encuentra al momento de ejecutarse
// Para cambiar modos de usuario se necesita especificar a cual como primer parámetro
// El simbolo + indica que se añade un modo, el símbolo - indica que se quita un modo
// En caso de que el modo que se quiera activar necesite parámetros deben ir separados por espacios
// La gestion de modos puede ir como un unico string o como varios parámetros separados por espacios
// Ejemplo: MODE #general +k -l pass
// Ejemplo: MODE #general +k pass +l 10
// Ejemplo: MODE #general +kl pass 10
// Ejemplo: MODE #general +kl-it pass 10
// Ejemplo: MODE #general +kl-it+o-o pass 10 user1 user2

void Bot::CmMode(t_msg &msg, int fd) {
	(void)msg;
    (void)fd;
// 	// Esto solo lo pueden hacer los admins si lo intenta un usuario normal dar error
// 	/* 1.- Si no envía más de 1 parámetro, enviar modos actuales del canal
// 	 * 2.- Comprobar que el canal al que se quiere establecer el modo exista
// 	 * 3.- Comprobar que el cliente que establece el modo sea admin del canal
// 	 * 4.- Separar los modos y sus parámetros (MODE <channel> +kl-it <pass> <maxusers>)
// 	 * 5.- Si no es admin, enviar error de no tener permisos
// 	 * 6.- Si es admin, establecer el modo y enviar mensaje a todos los usuarios del canal
// 	 * 7.- Si no se especifica modo, enviar el actual modo del canal
// 	 */
// 	if (msg.params.size() < 1 || (msg.params.size() == 1 && msg.params[0].size() <= 1)) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
// 		return ;
// 	}

// 	if (msg.params.size() == 1) {
// 		if (msg.params[0][0] == '#' || msg.params[0][0] == '&') {
// 			if (_channel.find(msg.params[0]) == _channel.end()) {
// 				answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
// 				return ;
// 			}
// 			std::string modes = _channel[msg.params[0]]->getMode();
// 			answerClient(fd, RPL_CHANNELMODEIS, msg.params[0], modes);
// 			return ;
// 		}
// 		else {
// 			// TODO: Pensar si vamos a permitir modos de usuario
// 			if (_clients[fd]->getNickname() != msg.params[0]) {
// 				answerClient(fd, ERR_USERSDONTMATCH, "", "Cannot change modes of other users");
// 				return ;
// 			}
// 			// std::string modes = _clients[user]->getModes();
// 			answerClient(fd, RPL_UMODEIS, _clients[fd]->getNickname(), "modes");
// 			return ;
// 		}
// 	}

// 	if (msg.params[0][0] == '#' || msg.params[0][0] == '&') {
// 		if (_channel.find(msg.params[0]) == _channel.end()) {
// 			answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
// 			return ;
// 		}
// 	}
// 	else {
// 		if (_clients[fd]->getNickname() != msg.params[0]) {
// 			answerClient(fd, ERR_USERSDONTMATCH, "", "Cannot change modes of other users");
// 			return ;
// 		}
// 		answerClient(fd, RPL_CHANNELMODEIS, _clients[fd]->getNickname(), "modes");
// 		return ;
// 	}

// 	if (msg.params[0] == "#general" || msg.params[0] == "&general") {
// 		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "Cannot change modes of the general channel");
// 		return ;
// 	}

// 	if (!_channel[msg.params[0]]->isAdmin(fd)) {
// 		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "You're not channel operator");
// 		return ;
// 	}

// 	std::vector<std::string> modes;
// 	std::vector<std::string> params;
// 	size_t modeCount = 0;
// 	for (size_t i = 1; i < msg.params.size(); ++i) {
// 		if ((msg.params[i][0] == '+' || msg.params[i][0] == '-') && msg.params[i].size() > 1) {
// 			std::string sign;
// 			bool valid = false;
// 			for (size_t j = 0; j < msg.params[i].size(); ++j) {
// 				valid = false;
// 				if (msg.params[i][j] == '+' || msg.params[i][j] == '-') {
// 					sign = msg.params[i][j];
// 					continue ;
// 				}
// 				if (isalpha(msg.params[i][j])) {
// 					if (sign.empty()) {
// 						answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
// 						return ;
// 					}
// 					if (msg.params[i][j] == 'k' || msg.params[i][j] == 'l' || msg.params[i][j] == 'i'
// 						|| msg.params[i][j] == 't' || msg.params[i][j] == 'o' || msg.params[i][j] == 'b') {
// 						if (((msg.params[i][j] == 'k' || msg.params[i][j] == 'l') && sign == "+")
// 							||  msg.params[i][j] == 'o' || msg.params[i][j] == 'b')
// 							modeCount++;
// 						valid = true;
// 						modes.push_back(sign + msg.params[i][j]);
// 					}
// 					else {
// 						std::string character;
// 						character += msg.params[i][j];
// 						answerClient(fd, ERR_UNKNOWNMODE, character, "Unknown mode");
// 						return ;
// 					}
// 				}
// 			}
// 			if (!valid) {
// 				answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Sign must be followed by a mode");
// 				return ;
// 			}
// 		}
// 		else if ((msg.params[i][0] == '+' || msg.params[i][0] == '-') && msg.params[i].size() == 1) {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
// 			return ;
// 		}
// 		else
// 			params.push_back(msg.params[i]);
// 	}

// 	if (modes.empty()) {
// 		answerClient(fd, RPL_CHANNELMODEIS, msg.params[0], _channel[msg.params[0]]->getMode());
// 		return ;
// 	}

// 	if (params.size() != modeCount) {
// 		size_t modeB = 0;
// 		if ((modeCount - params.size()) > 0) {
// 			for (size_t i = 0; i < modes.size(); ++i) {
// 				if (modes[i] == "+b")
// 					modeB++;
// 			}
// 		}
// 		if ((modeCount - params.size()) > modeB) {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
// 			return ;
// 		}
// 	}

// 	for (size_t i = 0; i < modes.size(); ++i) {
// 		std::cout << "Setting mode: " << modes[i] << std::endl;
// 		if (modes[i][0] == '+')
// 			manageAddMode(modes[i][1], msg.params[0], params, fd);
// 		else if (modes[i][0] == '-')
// 			manageRemoveMode(modes[i][1], msg.params[0], params, fd);
// 		else {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Mode must start with + or -");
// 			return ;
// 		}
// 	}
}

// void Server::manageAddMode(char mode, const std::string &channel, std::vector<std::string> &params, int fd) {
// 	if (_channel[channel]->hasMode(mode)) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Channel already has mode +" + std::string(1, mode));
// 		return ;
// 	}
	
// 	if (mode == 'i') {
// 		_channel[channel]->setMode('i');
// 		_channel[channel]->broadcastMessage(fd, "MODE", "", "+i");
// 		sendMsgToClient(fd, "MODE", channel, "+i");
// 		return ;
// 	}
// 	if (mode == 't') {
// 		_channel[channel]->setMode('t');
// 		_channel[channel]->broadcastMessage(fd, "MODE", "", "+t");
// 		sendMsgToClient(fd, "MODE", channel, "+t");
// 		return ;
// 	}
// 	if (mode == 'b') {
// 		if (params.empty()) {
// 			answerClient(fd, RPL_BANLIST, channel, _channel[channel]->listBanned());
// 			answerClient(fd, RPL_ENDOFBANLIST, channel, "End of /BAN list");
// 			return ;
// 		}
// 		int userFd = _channel[channel]->getUserFd(params[0]);
// 		if (userFd == -1) {
// 			answerClient(fd, ERR_NOSUCHNICK, params[0], "No such nick");
// 			return ;
// 		}
// 		if (_channel[channel]->isBanned(userFd)) {
// 			answerClient(fd, ERR_CHANOPRIVSNEEDED, channel, "User is already banned");
// 			return ;
// 		}
// 		Client *client = _clients[userFd];
// 		_channel[channel]->addBannedList(client);
// 		_channel[channel]->broadcastMessage(fd, "MODE", client->getNickname(), "+b " + client->getNickname());
// 		sendMsgToClient(fd, "MODE", channel, "+b " + client->getNickname());
// 		params.erase(params.begin());
// 		return ;
// 	}

// 	if (params.size() < 1) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode +" + std::string(1, mode));
// 		return ;
// 	}
// 	if (params[0].empty()) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Param cannot be empty for mode +" + std::string(1, mode));
// 		return ;
// 	}
// 	if (mode == 'k') {
// 		_channel[channel]->setPass(params[0]);
// 		_channel[channel]->setMode('k');
// 		_channel[channel]->broadcastMessage(fd, "MODE", "", "+k " + params[0]);
// 		sendMsgToClient(fd, "MODE", channel, "+k " + params[0]);
// 		params.erase(params.begin());
// 	}
// 	if (mode == 'l') {
// 		size_t numberUsers = strtol(params[0].c_str(), NULL, 10);
// 		if (numberUsers <= 0) {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Max users must be a positive number");
// 			return ;
// 		}
// 		if (numberUsers < _channel[channel]->getUserCount()) {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Max users cannot be less than current users");
// 			return ;
// 		}
// 		_channel[channel]->setMaxUsers(numberUsers);
// 		_channel[channel]->setMode('l');
// 		_channel[channel]->broadcastMessage(fd, "MODE", "", "+l " + to_string(numberUsers));
// 		sendMsgToClient(fd, "MODE", channel, "+l " + to_string(numberUsers));
// 		params.erase(params.begin());
// 	}
// 	if (mode == 'o') {
// 		int userFd = _channel[channel]->getUserFd(params[0]);
// 		if (userFd == -1) {
// 			answerClient(fd, ERR_NOSUCHNICK, params[0], "No such nick");
// 			return ;
// 		}
// 		if (_channel[channel]->isAdmin(userFd)) {
// 			answerClient(fd, ERR_CHANOPRIVSNEEDED, channel, "User is already admin");
// 			return ;
// 		}
// 		Client *client = _clients[userFd];
// 		_channel[channel]->addAdminList(client);
// 		_channel[channel]->broadcastMessage(fd, "MODE", client->getNickname(), "+o " + client->getNickname());
// 		sendMsgToClient(fd, "MODE", channel, "+o " + client->getNickname());
// 		msgClientToClient(fd, userFd, "MODE " + channel + " +o " + client->getNickname(), "");
// 		params.erase(params.begin());
// 	}
// }

// void Server::manageRemoveMode(char mode, const std::string &channel, std::vector<std::string> &params, int fd) {
// 	if (mode == 'i' || mode == 't' || mode == 'k' || mode == 'l') {
// 		if (!_channel[channel]->hasMode(mode)) {
// 			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Channel doesn't have mode -" + std::string(1, mode));
// 			return ;
// 		}
// 		if (mode == 'i')
// 			_channel[channel]->unsetMode('i');
// 		if (mode == 't')
// 			_channel[channel]->unsetMode('t');
// 		if (mode == 'k') {
// 			_channel[channel]->setPass("");
// 			_channel[channel]->unsetMode('k');
// 		}
// 		if (mode == 'l') {
// 			_channel[channel]->setMaxUsers(0);
// 			_channel[channel]->unsetMode('l');
// 		}
// 		_channel[channel]->broadcastMessage(fd, "MODE", "", "-" + std::string(1, mode));
// 		sendMsgToClient(fd, "MODE", channel, "-" + std::string(1, mode));
// 		return ;
// 	}

// 	if (params.size() < 1) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode -" + std::string(1, mode));
// 		return ;
// 	}
// 	if (params[0].empty()) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Param cannot be empty for mode -" + std::string(1, mode));
// 		return ;
// 	}

// 	if (mode == 'o') {
// 		int userFd = _channel[channel]->getUserFd(params[0]);
// 		if (userFd == -1) {
// 			answerClient(fd, ERR_NOSUCHNICK, params[0], "No such nick");
// 			return ;
// 		}
// 		if (!_channel[channel]->isAdmin(userFd)) {
// 			answerClient(fd, ERR_CHANOPRIVSNEEDED, channel, "User is not admin");
// 			return ;
// 		}
// 		_channel[channel]->removeAdminList(_clients[userFd]);
// 		_channel[channel]->broadcastMessage(fd, "MODE", _clients[userFd]->getNickname(), "-o " + _clients[userFd]->getNickname());
// 		sendMsgToClient(fd, "MODE", channel, "-o " + _clients[userFd]->getNickname());
// 		msgClientToClient(fd, userFd, "MODE " + channel + " -o " + _clients[userFd]->getNickname(), "");
// 		params.erase(params.begin());
// 	}
// 	if (mode == 'b') {
// 		int userFd = _channel[channel]->getUserFd(params[0]);
// 		if (userFd == -1) {
// 			answerClient(fd, ERR_NOSUCHNICK, params[0], "No such nick");
// 			return ;
// 		}
// 		if (!_channel[channel]->isBanned(userFd)) {
// 			answerClient(fd, ERR_CHANOPRIVSNEEDED, channel, "User is not banned");
// 			return ;
// 		}
// 		_channel[channel]->removeBannedList(_clients[userFd]);
// 		_channel[channel]->broadcastMessage(fd, "MODE", _clients[userFd]->getNickname(), "-b " + _clients[userFd]->getNickname());
// 		sendMsgToClient(fd, "MODE", channel, "-b " + _clients[userFd]->getNickname());
// 		params.erase(params.begin());
// 	}
// }
