#include <irc.hpp>
#include <Server.hpp>

int	parseArgs(const std::string &port, const std::string &password) 
{
	for (size_t i = 0; i < port.size(); ++i) {
		if (!std::isdigit(port[i])) {
			std::cerr << "Error: Port must be a unsigned number." << std::endl;
			return -1;
		}
		if (port[i] == '0' && i == 0) {
			std::cerr << "Error: Port cannot start with zero." << std::endl;
			return -1;
		}
		if (i > 5) {
			std::cerr << "Error: Port is too long." << std::endl;
			return -1;
		}
	}

	int check = std::atoi(port.c_str());
	if (check <= 1024 || check > 65535) {
		std::cerr << "Error: Port must be between 1024 and 65535." << std::endl;
		return -1;
	}

	if (password.empty()) {
		std::cerr << "Error: Password cannot be empty." << std::endl;
		return -1;
	}
	return check;
}

int main(int ac, char** av) {
	
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int port;
	if ((port = parseArgs(av[1], av[2])) == -1) 
		return 1;

	std::cout << "Starting server on port " << port << " with password '" << av[2] << "'." << std::endl;
	
	Server	Server(port, av[2]);

	return 0;
}