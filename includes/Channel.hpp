#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <irc.hpp>

class Channel
{
	private:
		int								_maxUsers;
		std::string						_name;
		std::map<std::string, Client *>	_users;
		std::map<std::string, Client *>	_admins;

		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

	public:

		Channel();
		~Channel();

		const std::string &getName() const;
		const int &getMaxUsers() const;

		void setName(const std::string &name);
		void setMaxUsers(const int &maxUsers);
};

#endif 