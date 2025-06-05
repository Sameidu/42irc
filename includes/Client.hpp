#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <irc.hpp>
# include <sys/socket.h>
# include <netinet/in.h>

class Client
{
	private:
		/* TODO: var status admin, normal user*/
		std::string	_userName;
		std::string _nickName;
		std::string _realName;
		const int	_clientFd;
		sockaddr_in	*_clientAddr;

	public:
		// Client(const int fd);
		Client(const int fd, sockaddr_in	*clientAddr);
		~Client();

		// Getters
		const std::string &getUsername() const;
		const std::string &getNickname() const;
		const std::string &getRealname() const;
		int getFd() const;

		// Setters
		void setUsername(const std::string &username);
		void setNickname(const std::string &nickname);
		void setRealname(const std::string &realname);

};

// TODO: Comprobar si es necesario compartir funciones.

#endif 