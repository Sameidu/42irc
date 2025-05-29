#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <irc.hpp>

class Client;
class Channel;

class Server
{
	private:
		int									_port;
		std::string							_password;
		std::map <std::string, Client *>	_clients;
		std::map <std::string, Channel *>	_channel;

		Server(const Server &other);
		Server &operator=(const Server &other);
		Server();
 
	public:

		Server(const int &port, const std::string &password);
		~Server();

		// Getters
		const int &getPort() const;
		const std::string &getPassword() const;

		// Setters
		void setPort(const int &port);
		void setPassword(const std::string &password);
};

#endif