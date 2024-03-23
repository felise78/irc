#include "Server.hpp"

Server::Server(int port, std::string const& password) : 
	_socket_fd(-1) {
	
	// THIS MUST BE PARSED FROM MAIN argv
	setServerHostName();
	setPort(port);
	setServerPassword(password);

	/* DEBUG */
	// std::cout << MAGENTA << "\tServer constructor called" << RESET << std::endl;
	// std::cout << YELLOW << "\tinitializing server socket [ listen ].." << RESET << std::endl;
	/* ***** */

	initSocket();
	connectToNetwork();
	startListenToNetwork();
}

Server::~Server() {
	// std::cout << RED << "\tServer destructor called" << RESET << std::endl;

	// This will leak when server is interrupted with ctrl+c AND `exit()` is used in signal handler..
	close(_socket_fd);
}

/*
** SETTERS
*/
void	Server::setPort(const int & port) {
	_port = port;
}

void	Server::setServerHostName() {

	char hostname[1024];
	gethostname(hostname, 1024);
	_serverHostName = hostname;
}

void	Server::setServerPassword(const std::string & serverPassword) {
	_serverPassword = serverPassword;
}

/*
** GETTERS
*/
int		Server::getServerFd() const {
	return (_socket_fd);
}

int		Server::getPort() const {
	return _port;
}

std::string const &	Server::getServerHostName() const {
	return _serverHostName;
}

std::string const&	Server::getServerPassword() const {
	return _serverPassword;
}

bool Server::passwordVerified(std::string const & pass) const {

	return (pass == _serverPassword);
}

void	Server::initSocket() {

	// Defining address structure
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);

	_socket_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP

	if (_socket_fd < 0) {
		perror("In Server::initSocket()");
		exit(EXIT_FAILURE);
	}

	/* DEBUG */
	// std::cout << GREEN << "\t[+] Socket created successfully. _socket_fd = " << this->_socket_fd << RESET << std::endl; 
	/* ***** */
}

void	Server::connectToNetwork() {

	int	bindStatus = -1;
	int opt = 1;

	// `SO_REUSEADDR` will allow to use socket right after the program exits without waiting for the operating system to clean up the socket
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << RED << "\t[-] Error setting socket options.. setsockopt() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	bindStatus = bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address));

	if (bindStatus < 0) {
		perror("In Server::connectToNetwork()");
		exit(EXIT_FAILURE);
	}
}

void	Server::startListenToNetwork() {

	int	listenReturn = listen(_socket_fd, BACKLOG);

	if (listenReturn < 0) {
		perror("In Server::startListenToNetwork()");
		exit(EXIT_FAILURE);
	}
}

/*
** This function is for testing purposes
** PRINTS THE SERVER DATA
*/
void	Server::printServerData() const {

	std::cout << BLUE << "[!] Server DATA:" << RESET << std::endl;
	std::cout << CYAN << "\tServer hostname:  " << RESET << _serverHostName << std::endl;
	std::cout << CYAN << "\tServer port:      " << RESET << _port << std::endl;
	std::cout << CYAN << "\tServer pass:      " << RESET << _serverPassword << std::endl;
	std::cout << CYAN << "\tServer socket fd: " << RESET << getServerFd() << std::endl;
}
