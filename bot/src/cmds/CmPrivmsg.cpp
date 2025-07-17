#include <Bot.hpp>

void Bot::CmPrivMsg(t_msg &msg)
{
	if (msg.from == "Bot")
		return ;

	std::vector<std::string> words;
	splitCmd(msg.trailing, words, ' ');

	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
		std::cout << "Word: " << *it << std::endl;
	}
	
	std::string cmd = words[0];
	std::string target;
	if (msg.params[0] == "Bot")
		target = msg.from;
	else
		target = msg.params[0];

	std::map<std::string, botCmd>::iterator it = _botCmds.find(cmd);
	if (cmd[0] == '!') {
		if (it != _botCmds.end()) {
			if (cmd == "!help")
				words.insert(words.begin(), msg.from);
			std::string response = (this->*(it->second))(words);
			if (cmd == "!say") 
				target = msg.from;
			if (!response.empty() && cmd != "!help" && (target[0] == '#' || target[0] == '&'))
				sendMsg("NOTICE", target, response);
			else if (!response.empty() && cmd != "!help")
				sendMsg("PRIVMSG", target, response);
		}
		else
			sendMsg("NOTICE", target, "Command not recognized, please type !<command>");
		return ;
	}
	
	if (msg.params[0] == "Bot")
		sendMsg("PRIVMSG", target, "Command not recognized, please type !<command>");
	else {
			for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
				if (*it == "!help") {
					sendMsg("NOTICE", msg.params[0], "For usage information, type '!help' or '!<command>'");
					 return ;
				}
			}
		}
}
