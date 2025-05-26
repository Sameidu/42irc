#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

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