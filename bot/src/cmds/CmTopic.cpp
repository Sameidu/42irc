#include <Bot.hpp>


// TOPIC <channel> [<topic>]
void Bot::CmTopic(t_msg &msg, int fd) {
	(void)msg;
    (void)fd;
	// // Modificar el valor solo lo pueden hacer los admins si esta activo modo 't'
	// /* 1.- Comprobar que los parametros sean como mucho 2
	//  * 2.- Comprobar que el canal al que se quiere establecer el topic exista
	//  * 3.- Comprobar que el cliente que establece el topic sea admin del canal
	//  * 4.- Si no es admin, enviar error de no tener permisos
	//  * 5.- Si es admin, establecer el topic y enviar mensaje a todos los usuarios del canal
	//  * 6.- Si no se especifica topic, enviar el actual topic del canal
	//  */
	// if (msg.params.size() < 1 || msg.params.size() > 2) {
	// 	answerClient(fd, ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
	// 	return ;
	// }
	// if (_channel.find(msg.params[0]) == _channel.end()) {
	// 	answerClient(fd, ERR_NOSUCHCHANNEL, msg.params[0], "No such channel");
	// 	return ;
	// }
	// if (!_channel[msg.params[0]]->hasUser(fd)) {
	// 	answerClient(fd, ERR_NOTONCHANNEL, msg.params[0], "You're not on that channel");
	// 	return ;
	// }

	// if (!msg.trailing.empty()) {
	// 	if (_channel[msg.params[0]]->hasMode('t') && !_channel[msg.params[0]]->isAdmin(fd)) {
	// 		answerClient(fd, ERR_CHANOPRIVSNEEDED, msg.params[0], "You're not channel operator");
	// 		return ;
	// 	}
	// 	_channel[msg.params[0]]->setTopic(msg.trailing);
	// 	_channel[msg.params[0]]->broadcastMessage(fd, "TOPIC", "", msg.trailing);
	// 	sendMsgToClient(fd, "TOPIC", msg.params[0], msg.trailing);
	// }
	// else {
	// 	if (_channel[msg.params[0]]->getTopic().empty())
	// 		answerClient(fd, RPL_NOTOPIC, msg.params[0], "No topic set");
	// 	else
	// 		answerClient(fd, RPL_TOPIC,  msg.params[0], _channel[msg.params[0]]->getTopic());
	// }
}