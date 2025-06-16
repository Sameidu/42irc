#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# define RED     "\033[91;1m"
# define GREEN   "\033[92;1m"
# define YELLOW  "\033[93;1m"
# define BLUE    "\033[94;1m"
# define PINK    "\033[95;1m"
# define CLEAR   "\033[0m"

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
# include <iostream>
# include <csignal>
# include <cstdio>
# include <utility>

# include <Client.hpp>
# include <Channel.hpp>

class Client;
class Channel;

# define	MAX_FDS	1024

/* CONNECT SUCCESS */
# define	RPL_WELCOME 			001

/* PASS */
# define 	ERR_PASSWDMISMATCH		464
# define	ERR_ALREADYREGISTRED	-1 // TODO no se que codigo es 

/* NICK */
# define	ERR_NONICKNAMEGIVEN		431 // Si no se da parámetro
# define	ERR_ERRONEUSNICKNAME	432 // Si el nick contiene caracteres inválidos
# define	ERR_NICKNAMEINUSE		433 // Si ya hay otro cliente con ese nick

/* COMMANDOS */
# define 	ERR_UNKNOWNCOMMAND		421 // comando desconocido

typedef struct	s_msg
{
	std::string					allMsg;
	std::string 				prefix;
	std::string					command;
	std::vector<std::string>	params;
	std::string 				trailing;
}  t_msg;


class Server
{
	private:
		typedef void (Server::*FCmd)(t_msg&, int);

		const int							_port;
		const std::string					_password;
		bool								_running;
		std::map <int, Client *>			_clients;
		std::map <std::string, Channel *>	_channel;
		int									_socketFd;
		sockaddr_in							_servAddr;
		int									_epollFd;
		std::map <std::string, FCmd>		_fCommands;
		
		void	connectNewClient();
		void	disconnectClient(int fd);
		t_msg	parseMsg(std::string msg);
		void	readMsg(int fd);
		void	manageServerInput();
		void	handleCommand(t_msg& msg, int fd);
		void	answerCLient(int fdClient, int code, std::string msg);
		void	initCmds();

		/* COMMANDS */
		void CmPass(t_msg& msg, int fd);
		void CmNick(t_msg& msg, int fd);
		void CmUser(t_msg& msg, int fd);

	public:

		Server(const int &port, const std::string &password);
		~Server();

		// Getters
		const int &getPort() const;
		const std::string &getPassword() const;

		void 	run();
		void 	init();
		void	createUserForClient(std::string args, std::string command, int fdClient);
};

bool	setNonBlocking(int fd);

#endif