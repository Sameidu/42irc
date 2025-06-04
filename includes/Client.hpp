#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <irc.hpp>

class Client
{
	private:
		/* TODO: var status admin, normal user*/
		std::string	_username;
		std::string _nickname;
		std::string _realname;
		std::string _password;
		int			_fd;

	public:

		Client();
		~Client();

		// Getters
		const std::string &getUsername() const;
		const std::string &getNickname() const;
		const std::string &getRealname() const;
		const std::string &getPassword() const;
		int getFd() const;

		// Setters
		void setUsername(const std::string &username);
		void setNickname(const std::string &nickname);
		void setRealname(const std::string &realname);
		void setPassword(const std::string &password);
		void setFd(int fd);

};

#endif 