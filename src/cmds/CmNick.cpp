#include <Server.hpp>

void Server::CmNick(t_msg& msg, int fdClient)
{
	if (msg.params.empty() && msg.trailing.size() == 0)
		answerClient(fdClient, ERR_NONICKNAMEGIVEN, "", "No nickname given"); // ok, no manda nada
	else if (msg.params.empty() && msg.trailing.size() != 0)
		msg.params[0] = msg.trailing;

	if (msg.params[0].size() > MAX_CHAR_NICKNAME)
		answerClient(fdClient, ERR_ERRONEUSNICKNAME, "", "Erroneus nickname"); // ok

	if (!isSpecial(msg.params[0][0]) && !isalpha(msg.params[0][0]))
		answerClient(fdClient, ERR_ERRONEUSNICKNAME, "", "Erroneus nickname");	// ok

	for (std::size_t i = 1; i < msg.params[0].size(); i++)
	{
		char c = msg.params[0][i];
		if (!isalpha(c) && !isSpecial(c) && !isdigit(c) && c != '-')
			answerClient(fdClient, ERR_ERRONEUSNICKNAME, "", "Erroneus nickname");	// ok
	}
	
	std::map<int, Client*>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first == fdClient)
			continue ;
		else if (it->second->getNickname() == msg.params[0])
			answerClient(fdClient, ERR_NICKNAMEINUSE, "", "Nickname is already in use"); // ok
	}

	if (_clients[fdClient]->getIsConnect() == 1)
		_clients[fdClient]->setIsConnect(_clients[fdClient]->getIsConnect() + 1);
    _clients[fdClient]->setNickname(msg.params[0]);

	
	/* TODO: cuando se cambia el nick hay que mandar un msg de confirmacion al cliente y un msg,
		de que se cambio su nick a todos los canales en los que esta*/
}