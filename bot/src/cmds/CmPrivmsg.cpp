#include <Bot.hpp>

void Bot::CmPrivMsg(t_msg &msg, int fd)
{
    std::vector<std::string> words;
    splitCmd(msg.trailing, words, ' ');
    std::cout << "Command PRIVMSG received, but not implemented yet." << std::endl;

    if (msg.params[0] == "Bot") {
        std::cout << "This is a private message." << std::endl;
        std::string response = "PRIVMSG " + msg.from + " :Hello, I am a bot!\r\n";
        send(fd, response.c_str(), response.size(), MSG_EOR);
    }
    else {
        std::cout << "This is a channel message." << std::endl;
        std::string response = "PRIVMSG " + msg.params[0] + " :Hello, I am a bot!\r\n";
        send(fd, response.c_str(), response.size(), MSG_EOR);
    }
}

