# include <Bot.hpp>

void Bot::CmJoin(t_msg &msg) {
	std::string response = "Hello";

	if (msg.from != "Bot")
		response += ", " + msg.from + "!";
	else
		response += "!";
	response += " Type '!help' for a list of commands or send me a DM.\r\n";
	sendMsg(msg.params[0], response);
}