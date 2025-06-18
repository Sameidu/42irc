#pragma once

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

# define MAX_BYTES_MSG      512
# define MAX_EVENTS         128
# define MAX_CHAR_NICKNAME  9
# define MAX_PASS_TRY       3

/* UTILS */
bool	setNonBlocking(int fd);
bool    isSpecial(char c);
std::string to_string(const T & value);

#endif 