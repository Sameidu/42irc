#include <Server.hpp>

void Server::CmCAP(t_msg& msg, int fd)
{
	if (msg.params.size() < 2 || msg.params[0] != "LS")
	{
		answerClient(fd, ERR_UNKNOWNCOMMAND, "", "Unknown command");
		return ;
	}
	answerClient(fd, 302, "", "multi-prefix");
}