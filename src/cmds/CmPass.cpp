#include <Server.hpp>

void	Server::CmPass(t_msg& msg, int fdClient)
{
	if (_password != msg.params[0])
	{
		answerClient(fdClient, ERR_PASSWDMISMATCH, "", "Password incorrect");
		std::cout << RED << "contraseña incorrecta" << CLEAR << std::endl;
	}
	else
		_clients[fdClient]->setRegistrationState(RS_PassValidated);
	/* TODO: No he verificado todo aun solo contraseña*/
}