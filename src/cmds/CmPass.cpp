#include <Server.hpp>

void	Server::CmPass(t_msg& msg, int fdClient)
{
	std::cout << "----------->>>>>>" << _password << std::endl;
	std::cout << "----------->>>>>>" << msg.params[0] << std::endl;
	if (_password != msg.params[0])
	{
		answerClient(fdClient, ERR_PASSWDMISMATCH, "", "Password incorrect");
		std::cout << RED << "contraseña incorrecta" << CLEAR << std::endl;
	}
	else
		_clients[fdClient]->setIsConnect(1);
	/* TODO: No he verificado todo aun solo contraseña*/
}