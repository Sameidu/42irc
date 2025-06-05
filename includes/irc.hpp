#pragma once

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

# define MAX_BYTES_MSG   512
# define MAX_EVENTS      128

/* UTILS */
bool	setNonBlocking(int fd);

#endif 