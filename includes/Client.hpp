#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <irc.hpp>

class Client
{
	private:
		/* TODO: var status admin, normal user*/

	public:

		Client();
		Client(const Client &other);
		~Client();

		Client &operator=(const Client &other);
};

#endif 