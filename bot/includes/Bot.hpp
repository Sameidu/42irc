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

# define MAX_BYTES_MSG  512

class Bot {
	private:
		int			_fd;
		std::string	_password;
		std::string	_nick;
		std::string	_user;
		std::string	_name;
		std::string	_serverIp;
		int			_serverPort;

		void connectToServer();
		void sendCredentials();
		void eventListen();

	public:
		Bot(const std::string &ip, const int &port, const std::string &password);
		~Bot();

		void start();
		// void connectToServer();
};