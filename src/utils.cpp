
#include <irc.hpp>
#include <fcntl.h> 
#include <ctime>
#include <string>

bool	setNonBlocking(int fd)
{
	/* Firts take the existing flags from the file */
	int	flags = fcntl(fd, F_GETFL);
	if (flags == -1)
		return false;
	
	/* Bind the existing flags with nonblocking flags */
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return false;
	
	return true;
}

bool isSpecial(char c)
{
    return (c == '[' || c == ']' || c == '\\' || c == '`' ||
            c == '_' || c == '^' || c == '{' || c == '|' );
}


std::string currentDateTimeString() {
    // 1) Obtén la hora actual con time(NULL)
    std::time_t t = time(NULL);

    // 2) Convierte a tm* con localtime (global, no std::)
    std::tm* tm_info = localtime(&t);

    // 3) Formatea con strftime (global)
    char buf[64];
    strftime(buf, sizeof(buf), "%b %d %Y at %H:%M:%S", tm_info);

    return std::string(buf);
}
