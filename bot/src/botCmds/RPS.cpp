# include <Bot.hpp>

std::string Bot::playRPS(std::vector<std::string> &words) {
	if (words.size() != 2)
		return ("Usage: !rps <choice>");
	
	std::string choices[3] = {"rock", "paper", "scissors"};
	std::string userChoice = words[1];
	
	if (userChoice != "rock" && userChoice != "paper" && userChoice != "scissors")
		return ("Please choose either 'rock', 'paper' or 'scissors'");
	
	int randomIndex = rand() % 3;
	std::string botChoice = choices[randomIndex];
	
	if (userChoice == botChoice)
		return ("It's a tie! You both chose " + userChoice + ".");
	else if ((userChoice == "rock" && botChoice == "scissors") ||
			 (userChoice == "paper" && botChoice == "rock") ||
			 (userChoice == "scissors" && botChoice == "paper"))
		return ("You win! You chose " + userChoice + ", the bot chose " + botChoice + ".");
	else
		return ("You lose! You chose " + userChoice + ", the bot chose " + botChoice + ".");

}