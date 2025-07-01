
#include <irc.hpp>
#include <fcntl.h> 

template<typename T>
std::string to_string(const T & value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

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

std::string currentDateTimeString() 
{
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%b %d %Y at %H:%M:%S", std::localtime(&t));
    return buf;
}
