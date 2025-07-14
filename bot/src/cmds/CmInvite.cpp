#include <Bot.hpp>

// INVITE <nickname> <channel>
void Bot::CmInvite(t_msg &msg, int fd) {
	std::cout << "Command INVITE received, but not implemented yet." << std::endl;
	std::string response = "JOIN " + msg.trailing + "\r\n";
	send(fd, response.c_str(), response.size(), MSG_EOR);
}
