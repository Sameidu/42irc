# include <Bot.hpp>

void Bot::CmJoin(t_msg &msg, int fd) {
	std::string response = "PRIVMSG " + msg.params[0] + " :Hello";

	if (msg.from != "Bot")
		response += ", " + msg.from + "!";
	else
		response += "!";
	response += " Type '!help' for a list of commands or send me a DM.\r\n";
	if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0) {
		std::cerr << "Error sending join message to " << msg.params[0] << std::endl;
		return;
	}
}