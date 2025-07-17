# include <Bot.hpp>

std::string Bot::helpMsg(std::vector<std::string> &words) {
	static const std::string help[10] = {
		"Available commands:",
		"!help - Show this help message <!help>",
		"!wisdom - Get a random piece of wisdom <!wisdom>",
		"!dice - Roll a dice <!dice>",
		"!rps - Play rock-paper-scissors <!rps <choice>>",
		"!say - Make the bot say something <!say <dest> <message>>",
		"!nones - Play odd or even <!nones <choice> <number>>",
		"!8ball - Ask the magic 8 ball a question <!8ball <question>>",
		"!guess - Guess a number between 1 and 100 <!guess <number>>",
		"!counter - Increment a counter <!counter>"
	};
	for (int i = 0; i < 10; ++i) {
		sendMsg("PRIVMSG", words[0], help[i]);
	}

	std::string response = "Sending help message to user DM";
	return response;
}
