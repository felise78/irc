#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <sstream>
	#include <map>
	#include <vector>
	//#include "../Channel/Channel.hpp"

	typedef enum Status {
		PASS_NEEDED,
		PASS_MATCHED, // pass correct no nick or user
		REGISTERED, // pass correct nick and user ok
		DELETED
	}	e_status;


	class Channel;

	class User {

		private :
		int								_port;		// not used in this class but needed by ServerManager
		int								_fd; 
		std::string						_hostName; // ..parsed in `UserRequestParsing` class..
		std::string 					_nickName;	// ..parsed in `UserRequestParsing` class..
		std::string 					_userName; // ..parsed in `UserRequestParsing` class..
		std::string						_realName; // ..parsed in `UserRequestParsing` class..
		std::string 					_password; // ..parsed in `UserRequestParsing` class..
		e_status						_status;
		// ..to use for composing the first response message to the client (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO..)

		public :

		// user status enum
		User();
		// User(const User& copy);
		// User& operator=(const User& src);
		~User();
		// attributs publics
		std::string						userMessageBuffer;
		std::string						responseBuffer;
		std::map<std::string, Channel*>	_channels;
		std::vector<std::string>		_privmsg_nicks;
		bool							_authenticated;
		bool							_handshaked;
		bool							_pinged;
		bool							_cap;
		bool							isBot;
		bool							skip_mode;

		// Setters //
		void	setStatus(e_status status);
		void	setPort(const int& port);
		void	setFd(const int& socket);
		void 	setNickName(const std::string& nickname);
		void 	setUserName(const std::string& username);
		void 	setHostName(const std::string& hostname);
		void	setRealName(const std::string& realname);
		void 	setPassword(const std::string& password);
		void	setChannel(Channel& channel);
		void	setAuthenticated(bool authenticated);
		void	setHandshaked(bool handshaked);
		void	setAsBot();
		void	setPinged(bool pinged);
		// Getters //
		e_status getStatus();
		const int& getFd( void ) const;
		const std::string& getNickName( void ) const;
		const std::string& getUserName( void ) const;
		const std::string& getHostName( void ) const;
		const std::string& getRealName() const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name );
		bool	authenticated();
		bool	handshaked();
		bool	pinged();

		std::string			getPrefix();
		void	removeChannel(const std::string& channelName);
	};

#endif