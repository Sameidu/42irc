# include <Bot.hpp>

std::string Bot::magic8Ball(std::vector<std::string> &words) {
	static const std::string magicBall[20] = {
		"It is certain",
		"It is decidedly so",
		"Without a doubt",
		"Yes definitely",
		"You may rely on it",
		"As I see it, yes",
		"Most likely",
		"Outlook good",
		"Yes",
		"Signs point to yes",
		"Reply hazy, try again",
		"Ask again later",
		"Better not tell you now",
		"Cannot predict now",
		"Concentrate and ask again",
		"Don't count on it",
		"My reply is no",
		"My sources say no",
		"Outlook not so good",
		"Very doubtful"
	};

	if (words.size() < 2)
		return ("Please ask a question.");
	int code = rand() % 20;
	return magicBall[code];
}