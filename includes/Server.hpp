#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <irc.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include <exception>
# include <unistd.h>
# include <cstring>
# include <poll.h> 

class Client;
class Channel;

#define	MAX_FDS	1024

class Server
{
	private:
		int									_port;
		std::string							_password;
		std::map <std::string, Client *>	_clients;
		std::map <std::string, Channel *>	_channel;
		int									_socketFd;
		sockaddr_in							_servAddr;
		struct pollfd						_fds[MAX_FDS];

		Server(const Server &other);
		Server &operator=(const Server &other);
		Server();
 
	public:

		Server(const int &port, const std::string &password);
		~Server();

		void run();
		// Getters
		const int &getPort() const;
		const std::string &getPassword() const;

		/* TODO: hacen falta? si los inicializamos en el constructor */
		// Setters 
		void setPort(const int &port);
		void setPassword(const std::string &password);
};

#endif