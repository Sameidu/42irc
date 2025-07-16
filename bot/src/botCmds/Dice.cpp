# include <Bot.hpp>

std::string Bot::rollDice(std::vector<std::string> &words) {
	(void)words;
	int randNb = rand() % 6;
	std::string response = "*🎲Sonidito de dados🎲*: ";
	return (response + to_string(randNb));
}