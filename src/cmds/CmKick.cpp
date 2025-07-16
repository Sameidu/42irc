#include <Server.hpp>

// KICK <channel> <user> *( "," <user> ) [<comment>]
void Server::CmKick(t_msg &msg, int fd) {
	// Esto solo lo pueden hacer los admins si lo intenta un usuario normal dar error
	/* 1.- Comprobar que los parametros sean como mucho 2 + trailing (canal, usuario, razon opcional)
	 * 2.- Comprobar que el canal del que se quiere expulsar exista
	 * 3.- Comprobar que el cliente que quiere expulsar a otro se encuentre en el canal
	 * 4.- Comprobar que el cliente que quiere expulsar es admin del canal
	 * 5.- Comprobar que el cliente que se quiere expulsar se encuentre en el canal
	 */
	if (msg.params.size() < 1 || msg.params.size() > 2) {
		answerClient(fd, ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
		return ;
	}
	if (_channel.find(msg.params[0]) == _channel.end()) {
		answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
		return ;
	}
	if (!_channel[msg.params[0]]->hasUser(fd)) {
		answerClient(fd, ERR_NOTONCHANNEL, msg.params[0], "You're not on that channel");
		return ;
	}
	if (!_channel[msg.params[0]]->isAdmin(fd)) {
		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "You're not channel operator");
		return ;
	}
	if (_channel[msg.params[0]]->getName() == "#general") {
		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "You can't kick users from #general");
		return ;
	}
	
	std::vector<std::string> users;
	splitCmd(msg.params[1], users, ',');
	for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it) {
		if (!_channel[msg.params[0]]->hasUser(*it)) {
			answerClient(fd, ERR_USERNOTINCHANNEL, msg.params[0], "User " + *it + " is not on that channel");
			continue ;
		}
		int userFd = _channel[msg.params[0]]->getUserFd(*it);
		if (!userFd)
			continue ;
		std::string reason = (!msg.trailing.empty()) ? msg.trailing : _clients[fd]->getNickname() + " kicked from " + msg.params[0];
		sendMsgToClient(fd, "KICK", msg.params[0] + " " + *it , reason);
		_channel[msg.params[0]]->broadcastMessage(fd, "KICK", *it, reason);
		_channel[msg.params[0]]->disconnectUser(_clients[userFd]);
		if (_channel[msg.params[0]]->getUserCount() == 0) {
			delete _channel[msg.params[0]];
			_channel.erase(msg.params[0]);
		}
	}
}