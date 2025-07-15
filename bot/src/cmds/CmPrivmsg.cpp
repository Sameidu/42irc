#include <Bot.hpp>

void Bot::CmPrivMsg(t_msg &msg, int fd)
{
    std::vector<std::string> words;
    splitCmd(msg.trailing, words, ' ');

    for (size_t i = 0; i < words.size(); ++i) {
        std::cout << "Word[" << i << "]: " << words[i] << std::endl;
    }

    if (msg.params[0] == "Bot") {
        std::string response = "PRIVMSG " + msg.from + " :";
        if (words[0][0] == '!') {
            if (words[0] == "!help")
                response += "Available commands: !help...\r\n";
            else
                response += "Command not recognized, please type !<command>\r\n";
        }
        else
            response += "Command not recognized, please type !<command>\r\n";
        if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0) {
            std::cerr << "Error sending private message to " << msg.params[0] << std::endl;
            return ;
        }
    }
    else {
        if (msg.from == "Bot")
            return ;
        std::string response;
        if (words.size() == 1 && words[0] == "!help") {
           response += "PRIVMSG " + msg.params[0] + " :" + "The available commands in channel are: !help...\r\n";   
        }
        else {
            for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
                if (*it == "!help") {
                    response += "PRIVMSG " + msg.params[0] + " :" + "For usage information, type '!help' or '!<command>'\r\n";
                    break;
                }
            }
        }
        if (send(fd, response.c_str(), response.size(), MSG_EOR) < 0) {
            std::cerr << "Error sending private message to " << msg.params[0] << std::endl;
            return;
        }
    }
}

