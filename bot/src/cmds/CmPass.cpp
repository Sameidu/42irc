#include <Bot.hpp>

void	Bot::CmPass(t_msg& msg, int fd)
{
	(void)msg;
    (void)fd;
	// if (_clients[fdClient]->getRegistrationState() == RS_Registered)
	// 	answerClient(fdClient, ERR_ALREADYREGISTERED, "",  "You may not reregister");
		
	// else if (msg.params.size() < 1)
	// 	answerClient(fdClient, ERR_NEEDMOREPARAMS, "",  "Not enough parameters");

	// else if (_password != msg.params[0])
	// 	answerClient(fdClient, ERR_PASSWDMISMATCH, "", "Password incorrect");

	// else
	// 	_clients[fdClient]->setRegistrationState(RS_PassValidated);
}