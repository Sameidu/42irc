#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <irc.hpp>
# include <set>
#include <Client.hpp>


class Channel
{
	private:
		size_t					_maxUsers;
		std::string				_name;
		std::string				_pass;
		std::string				_topic;
		int						_ownerFd;
		
		std::set<char>			_mode;
		std::map<int, Client *>	_banned;
		std::map<int, Client *>	_invited;
		std::map<int, Client *>	_users;
		std::map<int, Client *>	_admins;

	public:

		Channel(const std::string &name);
		Channel(const std::string &name, const std::string &pass, const int &maxUsers);
		~Channel();

		const std::string &getName() const;
		const std::string &getPass() const;
		const size_t &getMaxUsers() const;
		size_t getUserCount() const;

		void setName(const std::string &name);
		void setMaxUsers(const size_t &maxUsers);
		void setPass(const std::string &pass);

		void newChannelUser(Client *client);
		void disconnectUser(Client *client);
};

#endif 