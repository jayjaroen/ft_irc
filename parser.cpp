#include "parser.hpp"

Command::Command(){}
Command::~Command(){}

std::string	Command::commandcheck(const std::string input)
{
	// std::string	messagetype[] = {"CAP", "AUTHENTICATE", "PASS", "NICK", "USER", "PING",
	// 	"PONG", "OPER", "QUIT", "ERROR"};
	// std::string	channelmess[] = {"JOIN", "PART", "TOPIC", "NAME", "LIST", "INVITE", "KICK"};
	// std::string	servmess[] = {"MOTD", "VERSION", "ADMIN", "CONNECT", "LUSERS", "TIME",
	// 	"STATS", "HELP", "INFO", "MODE"};
	// std::string	usermess[] = {"PRIVMSG", "NOTICE", "WHO", "WHOIS", "WHOWAS"};
	// std::string	opermess[] = {"KILL", "REHASH", "RESTART", "SQUIT"};
	// for (size_t i = 0; i < 10; i++)
	// {
	// 	if (word == connectmess[i])
	// 	{
	// 		this->type = 1;
	// 		this->code = i;
	// 		return (word.length());
	// 	}
	// }
	// for (size_t i = 0; i < 7; i++)
	// {
	// 	if (word == channelmess[i])
	// 	{
	// 		this->type = 2;
	// 		this->code = i;
	// 		return (word.length());
	// 	}
	// }
	// for (size_t i = 0; i < 10; i++)
	// {
	// 	if (word == servmess[i])
	// 	{
	// 		this->type = 3;
	// 		this->code = i;
	// 		return (word.length());
	// 	}
	// }
	// for (size_t i = 0; i < 5; i++)
	// {
	// 	if (word == usermess[i])
	// 	{
	// 		this->type = 4;
	// 		this->code = i;
	// 		return (word.length());
	// 	}
	// }
	// for (size_t i = 0; i < 4; i++)
	// {
	// 	if (word == opermess[i])
	// 	{
	// 		this->type = 5;
	// 		this->code = i;
	// 		return (word.length());
	// 	}
	// }

	size_t	len = 0;
	while (!isspace(input[len]))
		len++;
	std::string	word = input.substr(0, len);
	std::string commlist[] = {"CAP", "CONNECT", "QUIT", "JOIN", "PART", "PING", "HELP",
		"NICK", "PASS", "TOPIC", "INVITE", "KICK", "PRIVMSG", "OPER", "MODE", "RESTART"};
	for (size_t i = 0; i < 16; i++)
	{
		if (word == commlist[i])
		{
			this->type = i;
			return word;
		}
	}
	return "";
}

void	Command::msgparser(const std::string input)
{
	std::string	icomm = commandcheck(input);
	if (icomm.empty())
		this->params[0][0] = input;
	else
	{
		size_t	block = 0;
		size_t	content = 0;
		size_t	last = icomm.length() + 1;
		for (size_t i = last; input[i] || input[i] == '\0'; i++)
		{
			if (input[i] == ',')
			{
				this->params[block][content] = input.substr(last, i - 1);
				content++;
				if (input[i + 1])
					last = i + 1;
			}
			else if (isspace(input[i]) && !isspace(input[i - 1]))
			{
				this->params[block][content] = input.substr(last, i - 1);
				block++;
				content = 0;
				if (input[i + 1])
					last = i + 1;
			}
			else if (input[i] == '\0')
			{
				if (!isspace(input[i - 1]))
				this->params[block][content] = input.substr(last, i - 1);
				break;
			}
		}
	}
}