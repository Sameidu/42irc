#pragma once

# include <irc.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Channel.hpp"

class Client
{
	private:
		std::string				_userName;
		std::string 			_nickName;
		std::string 			_realName;
		const int				_clientFd;
		sockaddr_in	*			_clientAddr;
		int						_isConnect;
		std::string				_bufferMsgClient;
		std::vector<Channel*>	_channels;

	public:
		Client(const int fd, sockaddr_in	*clientAddr);
		~Client();

		// Getters
		const std::string &getUsername() const;
		const std::string &getNickname() const;
		const std::string &getRealname() const;
		const int &getIsConnect() const;
		int getFd() const;
		std::string &getBufferMsgClient();
		std::vector<Channel*>& getChannels();

		// Setters
		void setUsername(const std::string &username);
		void setNickname(const std::string &nickname);
		void setRealname(const std::string &realname);
		void setIsConnect(const int &isConnect);
		void setBufferMsgClient(const std::string &msg);

};

// TODO: Comprobar si es necesario compartir funciones.
