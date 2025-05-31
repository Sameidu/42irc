
#include <irc.hpp>
#include <fcntl.h> 

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