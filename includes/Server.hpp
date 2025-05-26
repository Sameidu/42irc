#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <irc.hpp>

class Client;
class Channel;

class Server
{
	private:
		std::map <std::string, Client *>	_clients;
		int									_port;
		std::string							_password;
		/* TODO: chanels pendiente valorar si es util o no*/
		/* TODO: crear clase channel*/
		std::map <std::string, Channel *>	_channel;
		Server(const Server &other);
		Server &operator=(const Server &other);
 
	public:

		Server(char *port, char *password);
		Server();
		~Server();

};

#endif