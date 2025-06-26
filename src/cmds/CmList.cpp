#include <Server.hpp>

// LIST [<channel>{,<channel>}] [<elistcond>{,<elistcond>}] 
void Server::CmList(t_msg &msg, int fd) {
	// TODO: Meter parametros de comadno (Filstro de busqueda)
	if (msg.params.size() > 1) {
		answerClient(fd, ERR_UNKNOWNCOMMAND, "", "Too many parameters");
		return ;
	}
	answerClient(fd, RPL_LISTSTART, "", "Channel  Users  Name");
	if (_channel.empty()) {
		answerClient(fd, RPL_LISTEND, "", "End of /LIST");
		return ;
	}
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it) {
		std::string response = it->first + " " + to_string(it->second->getUserCount()) + " " + it->second->getName();
		answerClient(fd, RPL_LISTITEM, it->first, response);
	}
	answerClient(fd, RPL_LISTEND, "", "End of /LIST");
}