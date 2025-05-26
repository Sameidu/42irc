#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <irc.hpp>

class Channel
{
	private:

	public:

		Channel();
		Channel(const Channel &other);
		~Channel();

		Channel &operator=(const Channel &other);
};

#endif 