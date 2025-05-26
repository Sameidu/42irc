#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Server
{
	private:

	public:

		Server();
		Server(const Server &other);
		~Server();

		Server &operator=(const Server &other);
};

#endif