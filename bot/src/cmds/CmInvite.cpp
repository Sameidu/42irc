#include <Bot.hpp>

// INVITE <nickname> <channel>
void Bot::CmInvite(t_msg &msg) {
	std::string response = "JOIN " + msg.trailing + "\r\n";
	if (send(_fd, response.c_str(), response.size(), MSG_EOR) < 0)
		return;
}
