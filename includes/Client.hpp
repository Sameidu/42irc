#pragma once

# include <irc.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Channel.hpp"

class Channel;

enum RegistrationStatus {
    RS_NoPass = 0,        // Aún no ha enviado PASS
    RS_PassValidated,     // PASS correcto recibido
    RS_NickValidated,     // NICK recibido
    RS_Registered         // USER recibido → registro completo
};

class Client
{
	private:
		std::string				_userName;
		std::string 			_nickName;
		std::string 			_realName;
		const int				_clientFd;
		sockaddr_in	*			_clientAddr;
		RegistrationStatus		_registrationState;
		std::string				_bufferMsgClient;
		std::vector<Channel*>	_channels;

	public:
		Client(const int fd, sockaddr_in	*clientAddr);
		~Client();

		// Getters
		const std::string &getUsername() const;
		const std::string &getNickname() const;
		const std::string &getRealname() const;
		const RegistrationStatus &getRegistrationState() const;
		int getFd() const;
		std::string &getBufferMsgClient();
		std::vector<Channel*>& getChannels();

		// Setters
		void setUsername(const std::string &username);
		void setNickname(const std::string &nickname);
		void setRealname(const std::string &realname);
		void setRegistrationState(RegistrationStatus st);
		void setBufferMsgClient(const std::string &msg);

    	void joinChannel(Channel* ch);
    	void leaveChannel(Channel* ch);

};

// TODO: Comprobar si es necesario compartir funciones.
