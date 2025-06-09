#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <irc.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include <exception>
# include <unistd.h>
# include <cstring>
# include <fcntl.h>
# include <cerrno>
# include <sys/epoll.h>
# include <arpa/inet.h>

# include <Client.hpp>
# include <Channel.hpp>

class Client;
class Channel;

# define	MAX_FDS	1024

class Server
{
	private:
		const int							_port;
		const std::string					_password;
		std::map <int, Client *>			_clients;
		std::map <std::string, Channel *>	_channel;
		int									_socketFd;
		sockaddr_in							_servAddr;
		int									_epollFd;
 
	public:

		Server(const int &port, const std::string &password);
		~Server();

		// Getters
		const int &getPort() const;
		const std::string &getPassword() const;

		void 	run();
		void 	init();
		void	connectNewClient();
		void	parseMsg(std::string msg, int fdClient);
		void	readMsg(epoll_event events);
		void	createUserForClient(std::string args, std::string command, int fdClient);
		bool	isCorrectNickname(std::string arg, int fdClient);
};

bool	setNonBlocking(int fd);

#endif