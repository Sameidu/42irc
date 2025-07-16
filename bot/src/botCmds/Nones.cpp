# include <Bot.hpp>

std::string Bot::oddEven(std::vector<std::string> &words) {
	if (words.size() != 3) 
		return ("Usage: !nones <choice> <number>\r\n");
	
	int number = stoi(words[2]);
	if (number < 0 || number > 100)
		return ("Please choose a number between 0 and 100");
	if (words[1] != "odd" && words[1] != "even")
		return ("Please choose either 'odd' or 'even'");
	
	int randomNumber = rand() % 10;
	std::string result = (randomNumber + number) % 2 == 0 ? "even" : "odd";
	std::string response = "You chose " + words[2] + ", the random number is " + to_string(randomNumber) + 
		", the result is " + result + ". ";
	
	if ((result == "even" && words[1] == "even") || (result == "odd" && words[1] == "odd"))
		response += "You win!";
	else
		response += "You lose!";
		
	return response;
}