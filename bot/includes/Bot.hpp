# pragma once

#ifndef BOT_HPP
# define BOT_HPP

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

extern int sckt;

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
		typedef void (Bot::*FCmd)(t_msg &);
		typedef std::string (Bot::*botCmd)(std::vector<std::string> &);

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
		std::map<std::string, botCmd> _botCmds;

		void connectToServer();
		void sendCredentials();
		void initCmds();
		void initBotCmds();
		void readMsg();
		void sendMsg(std::string &to, const std::string &msg);
		void handleCommand(t_msg &msg);
		t_msg parseMsg(std::string fullMsg);

		void CmInvite(t_msg &msg);
		void CmPrivMsg(t_msg &msg);
		void CmJoin(t_msg &msg);

		std::string helpMsg(std::vector<std::string> &words);
		std::string enlightenMe(std::vector<std::string> &words);
		std::string rollDice(std::vector<std::string> &words);
		std::string playRPS(std::vector<std::string> &words);
		std::string oddEven(std::vector<std::string> &words);
		std::string magic8Ball(std::vector<std::string> &words);
		std::string saySomething(std::vector<std::string> &words);

	public:
		Bot(const std::string &ip, const int &port, const std::string &password);
		~Bot();

		void start();
};

template<typename T>
static int stoi( const T & s ) {
    int i;
    std::istringstream(s) >> i;
    return i;
}

template<typename T>
std::string to_string(const T &value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T>
void splitCmd(const std::string &cmd, T &result, const char del) {
	size_t start = 0;
	size_t comma;
	while ((comma = cmd.find(del, start)) != std::string::npos) {
		if (comma > start) {
			std::string token = cmd.substr(start, comma - start);
			if (!token.empty())
				result.push_back(token);
		}
		start = comma + 1;
	}
	if (start < cmd.size()) {
		std::string token = cmd.substr(start);
		if (!token.empty())
			result.push_back(token);
	}
}

#endif

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