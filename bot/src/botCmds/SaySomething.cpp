# include <Bot.hpp>

std::string Bot::saySomething(std::vector<std::string> &words) {
	if (words.size() < 3)
		return ("Usage: !say <dest> <message>");

	words.erase(words.begin());
	if (words[0] == "Bot")
		return ("You cannot send a message to the bot itself.");

	std::string dest = words[0];
	words.erase(words.begin());
	std::string message;

	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
		if (it != words.begin())
			message += " ";
		message += *it;
	}
	if (dest[0] == '#' || dest[0] == '&')
		sendMsg("NOTICE", dest, message);
	else
		sendMsg("PRIVMSG", dest, message);
	return ("Message sent to " + dest + ": " + message);
}