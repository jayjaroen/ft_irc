#include "parser.hpp"

Command::Command() : type(0) {}
Command::~Command(){}

std::string	Command::commandcheck(const std::string input)
{
	size_t	len = 0;
	while (input[len] && !isspace(input[len]))
		len++;
	std::string	word = input.substr(0, len);
	std::string commlist[] = {"CAP", "USER", "QUIT", "JOIN", "PART", "PING", "HELP",
		"NICK", "PASS", "TOPIC", "INVITE", "KICK", "PRIVMSG", "OPER", "MODE", "RESTART"};
	for (size_t i = 0; i < 16; i++)
	{
		if (word == commlist[i])
		{
			this->type = i + 1;
			return word;
		}
	}
	return "";
}

void	Command::msgparser(const std::string input)
{
	std::string	icomm = commandcheck(input);
	std::vector<std::string>	content;
	if (icomm.empty())
	{
		content.push_back(input);
		this->params.push_back(content);
		content.clear();
	}
	else
	{
		if (!(this->params.empty()))
			return;
		if (this->lexer(input, icomm, this->params) == FAILURE)
			return;
		
		for (size_t i = 0; i < this->params.size(); i++)
		{
			for (size_t j = 0; j < this->params[i].size(); j++)
			{
				std::cout << this->params[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
}

std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}