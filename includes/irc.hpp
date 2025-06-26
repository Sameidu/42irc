#pragma once

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <iomanip>
# include <vector>
# include <map>
# include <algorithm>
# include <string>

# include <sys/socket.h>
# include <netinet/in.h>
# include <exception>
# include <unistd.h>
# include <cstring>
# include <fcntl.h>
# include <cerrno>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <iostream>
# include <csignal>
# include <cstdio>
# include <utility>
# include <sstream>

// COLORS
# define RED     "\033[91;1m"
# define GREEN   "\033[92;1m"
# define YELLOW  "\033[93;1m"
# define BLUE    "\033[94;1m"
# define PINK    "\033[95;1m"
# define CLEAR   "\033[0m"

# define MAX_BYTES_MSG      512
# define MAX_EVENTS         128
# define MAX_CHAR_NICKNAME  9
# define MAX_PASS_TRY       3

# define	MAX_FDS	1024

/* CONNECT SUCCESS */
# define	RPL_WELCOME 			001

/* PASS */
# define 	ERR_PASSWDMISMATCH		464
# define	ERR_ALREADYREGISTRED	-1 // TODO no se que codigo es 

/* NICK */
# define	ERR_NONICKNAMEGIVEN		431 // Si no se da parámetro
# define	ERR_ERRONEUSNICKNAME	432 // Si el nick contiene caracteres inválidos
# define	ERR_NICKNAMEINUSE		433 // Si ya hay otro cliente con ese nick

/* JOIN */
# define ERR_NEEDMOREPARAMS			461
# define ERR_NOSUCHCHANNEL 			403
# define ERR_ALREADYONCHANNEL		443
# define ERR_TOOMANYCHANNELS		405
# define ERR_BADCHANNELKEY			475
# define ERR_BANNEDFROMCHAN			474
# define ERR_CHANNELISFULL			471
# define ERR_INVITEONLYCHAN			473
# define ERR_BADCHANMASK			476
# define RPL_TOPIC					332
# define RPL_TOPICWHOTIME			333
# define RPL_NAMREPLY				353
# define RPL_ENDOFNAMES				366

/* PART */
# define	ERR_NOTONCHANNEL		442 // Si no estás en el canal
# define	ERR_NOSUCHCHANNEL		403 // Si el canal no existe
# define	ERR_USERNOTINCHANNEL	441

/* LIST */
# define	RPL_LISTSTART			321
# define	RPL_LISTITEM			322
# define	RPL_LISTEND				323

/* TOPIC */
# define ERR_CHANOPRIVSNEEDED		482
# define RPL_NOTOPIC				331
# define RPL_TOPIC					332
# define RPL_TOPICWHOTIME			333

/* COMMANDOS */
# define 	ERR_UNKNOWNCOMMAND		421 // comando desconocido

/* UTILS */
bool	setNonBlocking(int fd);

#endif 