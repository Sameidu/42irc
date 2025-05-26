#include <irc.hpp>
#include <Server.hpp>


/* TODO: retornar el puerto o -1 si da error */
bool	parseArgs(char** av)
{
	int port = std::atoi(av[1]);
	
	/* TODO: atoi no parsea bien un int corregir*/
	if (port <= 1024 || port > 65535) {
		std::cerr << "Error: Port number must be between 1024 and 65535." << std::endl;
		return false;
	}

	std::string password = av[2];
	if (password.empty()) {
		std::cerr << "Error: Password cannot be empty." << std::endl;
		return false;
	}
	else
		std::cout << "Starting server on port " << port << " with password '" << password << "'." << std::endl;
		return true;


}

int main(int ac, char** av) {
	
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}


	if (!parseArgs(av))
		return 1;
	
	/* TODO: pasar un int */
	Server	Server(av[1], av[2]);

	return 0;
}