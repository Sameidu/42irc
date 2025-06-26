#include <Server.hpp>

void	Server::CmUser(t_msg& msg, int fdClient)
{
	if (_clients[fdClient]->getIsConnect() == 2)
		_clients[fdClient]->setIsConnect(_clients[fdClient]->getIsConnect() + 1);
	_clients[fdClient]->setUsername(msg.params[0]);
}