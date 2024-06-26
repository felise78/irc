#include "Request.hpp"

/*CONSTRUCTORS/DESTRUCTORS*/
Request::Request(ServerManager& server, std::string buffer) : _server(server), _input_buffer(buffer), _request_valid(false)
{
	// if (DEBUG)
	// 	std::cout << "Request constructor called" << std::endl;
	parse_args();
}

Request::~Request()
{
	// if (DEBUG)
	// 	std::cout << "Request destructor called" << std::endl;
}

/*SETTERS*/

void	Request::parse_args()
{
    if (_input_buffer.empty())
		return ;
	if (_input_buffer[0] == '/') //trimming initial slash, if it exists, and \r\n
		_input_buffer.erase(_input_buffer.begin());
	for (size_t i = _input_buffer.find_last_of("\r\n"); i != std::string::npos;)
	{
		_input_buffer.erase(i);
		i = _input_buffer.find_last_of("\r\n");
	}
	if (_input_buffer.empty()) // si rien dans le buffer
		_input_map.insert(std::make_pair("command", "NONE")); //
	size_t i = _input_buffer.find_first_of(' ');
	if (i != std::string::npos)
	{
		_input_map.insert(std::make_pair("command", _input_buffer.substr(0, i)));
		_input_map.insert(std::make_pair("params", _input_buffer.substr(i + 1)));
	}
	else
		_input_map.insert(std::make_pair("command", _input_buffer));
	
	check_command_valid(_input_map["command"]);
	// if (_input_map.find("params") == _input_map.end() || _input_map["params"].empty())
	// {
	// 	//user.responseBuffer = ERR_NEEDMOREPARAMS(_input_map["command"]); /// pas de user dans Request
	// 	return;
	// }
}

/* UTILS */

void	Request::check_command_valid(std::string& command)
{
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);
	std::vector<std::string> allCommands;
    allCommands.push_back("KICK");
    allCommands.push_back("INVITE");
    allCommands.push_back("TOPIC");
    allCommands.push_back("CAP");
    allCommands.push_back("MODE");
    allCommands.push_back("NICK");
    allCommands.push_back("USER");
    allCommands.push_back("PASS");
    allCommands.push_back("OPER");
    allCommands.push_back("JOIN");
    allCommands.push_back("PRIVMSG");
    allCommands.push_back("PART");
	allCommands.push_back("PING");
	vector<string>::iterator it = allCommands.begin();
	for (; it != allCommands.end(); it++)
	{
		if (command == *it)
		{
			_request_valid = true;
			return ;
		}
	}
}

/*GETTERS*/

bool	Request::getRequestValid() const
{
	return _request_valid;
}

std::string const&	Request::getCommand() const
{
    std::map<std::string, std::string>::const_iterator it = _input_map.find("command");
    if (it != _input_map.end())
        return (it->second);
	else
		throw std::runtime_error("no command found");
}

std::map<std::string, std::string>& Request::getRequestMap()
{
	return (_input_map);
}

/*DEBUG*/
void	Request::print_map() const
{
	std::cout << "Printing input map:" << std::endl;
	std::map<std::string, std::string>::const_iterator it;
	for (it = _input_map.begin(); it != _input_map.end(); ++it)
	{
		std::cout << "Key: " << it->first << "\nValue: " << it->second << std::endl;
	}
}

void	Request::print_vector(std::vector<std::string> const& split_buffer)
{
	for (std::vector<std::string>::const_iterator it = split_buffer.begin(); it != split_buffer.end(); it++)
		std::cout << *it << std::endl;
}

