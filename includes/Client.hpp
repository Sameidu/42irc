#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

class Client
{
	private:

	public:

		Client();
		Client(const Client &other);
		~Client();

		Client &operator=(const Client &other);
};

#endif 