# include <Bot.hpp>

int	parseArgs(const std::string &ip, const std::string &port, const std::string &password) {
	if (ip.empty() || (inet_addr(ip.c_str()) == INADDR_NONE && ip != "localhost"))
		throw std::invalid_argument("Invalid IP address.");

	for (size_t i = 0; i < port.size(); ++i) {
		if (!std::isdigit(port[i]))
			throw std::invalid_argument("Port must be a number.");
		if (port[i] == '0' && i == 0)
			throw std::invalid_argument("Port cannot start with zero.");
		if (i > 5)
			throw std::length_error("Port is too long.");
	}

	int check = std::atoi(port.c_str());
	if (check <= 1024 || check > 65535)
		throw std::out_of_range("Port must be between 1024 and 65535.");
	if (password.empty())
		throw std::invalid_argument("Password cannot be empty.");
	return check;
}

int main(int argc, char **argv) {
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <ip> <port> <password>" << std::endl;
		return 1;
	}

	try {
		int port = parseArgs(argv[1], argv[2], argv[3]);
		Bot bot(argv[1], port, argv[3]);
		bot.start();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}