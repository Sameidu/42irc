# pragma once

// # include <irc.hpp>
// # include <Server.hpp>
// # include <Client.hpp>
// # include <Channel.hpp>

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

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
# include <sstream>

# define RED     "\033[91;1m"
# define GREEN   "\033[92;1m"
# define YELLOW  "\033[93;1m"
# define BLUE    "\033[94;1m"
# define PINK    "\033[95;1m"
# define CLEAR   "\033[0m"

# define MAX_BYTES_MSG  512

typedef struct	s_msg
{
	std::string					allMsg;
	std::string 				prefix;
	std::string					from;
	std::string					command;
	int							code;
	std::vector<std::string>	params;
	std::string					trailing;
	bool						hasTrailing;
}  t_msg;


class Bot {
	private:
		typedef void (Bot::*FCmd)(t_msg &, int);

		int			_fd;
		bool		_running;
		std::string	_password;
		std::string	_nick;
		std::string	_user;
		std::string	_name;
		std::string	_bufferMsg;
		std::string	_serverIp;
		int			_serverPort;
		std::map<std::string, FCmd> _cmds;

		void connectToServer();
		void sendCredentials();
		void initCmds();
		void readMsg(int fd);
		void handleCommand(t_msg &msg, int fd);
		//void sendMsg(const std::string &msg);
		t_msg parseMsg(std::string fullMsg);

		void CmCAP(t_msg& msg, int fd);
		void CmInvite(t_msg &msg, int fd);
		void CmJoin(t_msg &msg, int fd);
		void CmKick(t_msg &msg, int fd);
		void CmList(t_msg &msg, int fd);
		void CmMode(t_msg &msg, int fd);
		void CmNames(t_msg &msg, int fd);
		void CmNick(t_msg &msg, int fd);
		void CmPart(t_msg &msg, int fd);
		void CmPass(t_msg &msg, int fd);
		void CmPrivMsg(t_msg &msg, int fd);
		void CmTopic(t_msg &msg, int fd);
		void CmUser(t_msg &msg, int fd);

	public:
		Bot(const std::string &ip, const int &port, const std::string &password);
		~Bot();

		void start();
		// void connectToServer();
};

template<typename T>
void splitCmd(const std::string &cmd, T &result, const char del) {
	size_t start = 0;
	size_t comma;
	while ((comma = cmd.find(del, start)) != std::string::npos) {
		result.push_back(cmd.substr(start, comma - start));
		start = comma + 1;
	}
	result.push_back(cmd.substr(start));
}