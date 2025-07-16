#include <Bot.hpp>

// INVITE <nickname> <channel>
<<<<<<< HEAD
void Bot::CmInvite(t_msg &msg, int fd) {
	std::cout << "Command INVITE received, but not implemented yet." << std::endl;
	std::string response = "JOIN " + msg.trailing + "\r\n";
	send(fd, response.c_str(), response.size(), MSG_EOR);
=======
void Bot::CmInvite(t_msg &msg) {
	std::string response = "JOIN " + msg.trailing + "\r\n";
	if (send(_fd, response.c_str(), response.size(), MSG_EOR) < 0)
		return;
>>>>>>> origin/bot
}
