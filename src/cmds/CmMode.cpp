#include <Server.hpp>

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


void Server::CmMode(t_msg &msg, int fd) {
	// Esto solo lo pueden hacer los admins si lo intenta un usuario normal dar error
	/* 1.- Si no envía más de 1 parámetro, enviar modos actuales del canal
	 * 2.- Comprobar que el canal al que se quiere establecer el modo exista
	 * 3.- Comprobar que el cliente que establece el modo sea admin del canal
	 * 4.- Separar los modos y sus parámetros (MODE <channel> +kl-it <pass> <maxusers>)
	 * 5.- Si no es admin, enviar error de no tener permisos
	 * 6.- Si es admin, establecer el modo y enviar mensaje a todos los usuarios del canal
	 * 7.- Si no se especifica modo, enviar el actual modo del canal
	 */
	if (msg.params.size() < 1) {
		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
		return ;
	}

	if (msg.params.size() == 1) {
		if (msg.params[0][0] == '#' || msg.params[0][0] == '&') {
			if (_channel.find(msg.params[0]) == _channel.end()) {
				answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
				return ;
			}
			std::string modes = _channel[msg.params[0]]->getMode();
			answerClient(fd, RPL_CHANNELMODEIS, msg.params[0], modes);
			return ;
		}
		else {
			// TODO: Pensar si vamos a permitir modos de usuario
			if (_clients[fd]->getNickname() != msg.params[0]) {
				answerClient(fd, ERR_USERSDONTMATCH, "", "Cannot change modes of other users");
				return ;
			}
			// std::string modes = _clients[user]->getModes();
			answerClient(fd, RPL_CHANNELMODEIS, _clients[fd]->getNickname(), "modes");
			return ;
		}
	}

	if (msg.params[0][0] == '#' || msg.params[0][0] == '&') {
		if (_channel.find(msg.params[0]) == _channel.end()) {
			answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
			return ;
		}
	}
	else {
		if (_clients[fd]->getNickname() != msg.params[0]) {
			answerClient(fd, ERR_USERSDONTMATCH, "", "Cannot change modes of other users");
			return ;
		}
		answerClient(fd, RPL_CHANNELMODEIS, _clients[fd]->getNickname(), "modes");
		return ;
	}

	if (!_channel[msg.params[0]]->isAdmin(fd)) {
		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "You're not channel operator");
		return ;
	}

	std::string modes;
	std::vector<std::string> params;
	int modeCount = 0;
	for (size_t i = 1; i < msg.params.size(); ++i) {
		if ((msg.params[i][0] == '+' || msg.params[i][0] == '-') && msg.params[i].size() > 1) {
			std::string sign;
			bool valid = false;
			for (size_t j = 0; j < msg.params[i].size(); ++j) {
				if (msg.params[i][j] == '+' || msg.params[i][j] == '-') {
					sign = msg.params[i][j];
					continue ;
				}
				if (isalpha(msg.params[i][j])) {
					if (sign.empty()) {
						answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
						return ;
					}
					if (msg.params[i][j] == 'k' || msg.params[i][j] == 'l' || msg.params[i][j] == 'i'
						|| msg.params[i][j] == 't' || msg.params[i][j] == 'o' || msg.params[i][j] == 'b') {
							if (msg.params[i][j] == 'k' || msg.params[i][j] == 'l'
								||  msg.params[i][j] == 'o' || msg.params[i][j] == 'b')
								modeCount++;
							valid = true;
							modes += sign + msg.params[i][j];
						}
					else {
						answerClient(fd, ERR_UNKNOWNMODE, msg.params[i][j], "Unknown mode");
						return ;
					}
				}
			}
			if (!valid) {
				answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Sign must be followed by a mode");
				return ;
			}
		}
		else if ((msg.params[i][0] == '+' || msg.params[i][0] == '-') && msg.params[i].size() == 1) {
			answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
			return ;
		}
		else
			params.push_back(msg.params[i]);
	}

	if (modes.empty()) {
		answerClient(fd, RPL_CHANNELMODEIS, msg.params[0], _channel[msg.params[0]]->getMode());
		return ;
	}

	if (params.size() != modeCount) {
		answerClient(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters for mode");
		return ;
	}

	// for (size_t i = 0; i < modes.size(); ++i) {
	// 	int status;
	// 	if (modes[i] == '+' || modes[i] == '-') {
	// 		if (modes)
	// 	}
	// }
}