#include <Server.hpp>

int main(int ac, char** av) {
	
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int port = std::atoi(av[1]);
	
	if (port <= 0 || port > 65535) {
		std::cerr << "Error: Port number must be between 1 and 65535." << std::endl;
		return 1;
	}

	std::string password = av[2];
	if (password.empty()) {
		std::cerr << "Error: Password cannot be empty." << std::endl;
		return 1;
	}

	std::cout << "Starting server on port " << port << " with password '" << password << "'." << std::endl;

	return 0;
}