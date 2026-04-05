#include "parser.hpp"

Command::Command(){}
Command::~Command(){}

std::string	commandcheck(std::string input)
{
	size_t	len = 0;
	while (!isspace(input[len]))
		len++;
	std::string	word = input.substr(0, len);
	std::string commlist[] = {"CAP", "PASS", "NICK", "USER", "QUIT", "PING", "JOIN",
		"PART", "CONNECT", "PRIVMSG", "MODE", "KICK"};
	for (size_t i = 0; i < 12; i++)
	{
		if (word == commlist[i])
			return word;
	}
	return "";
}

size_t	Command::idcomms(const std::string word)
{
	std::string	connectmess[] = {"CAP", "AUTHENTICATE", "PASS", "NICK", "USER", "PING",
		"PONG", "OPER", "QUIT", "ERROR"};
	std::string	channelmess[] = {"JOIN", "PART", "TOPIC", "NAME", "LIST", "INVITE", "KICK"};
	std::string	servmess[] = {"MOTD", "VERSION", "ADMIN", "CONNECT", "LUSERS", "TIME",
		"STATS", "HELP", "INFO", "MODE"};
	std::string	usermess[] = {"PRIVMSG", "NOTICE", "WHO", "WHOIS", "WHOWAS"};
	std::string	opermess[] = {"KILL", "REHASH", "RESTART", "SQUIT"};

	for (size_t i = 0; i < 10; i++)
	{
		if (word == connectmess[i])
		{
			this->type = 1;
			this->code = i;
			return (word.length());
		}
	}
	for (size_t i = 0; i < 7; i++)
	{
		if (word == channelmess[i])
		{
			this->type = 2;
			this->code = i;
			return (word.length());
		}
	}
	for (size_t i = 0; i < 10; i++)
	{
		if (word == servmess[i])
		{
			this->type = 3;
			this->code = i;
			return (word.length());
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		if (word == usermess[i])
		{
			this->type = 4;
			this->code = i;
			return (word.length());
		}
	}
	for (size_t i = 0; i < 4; i++)
	{
		if (word == opermess[i])
		{
			this->type = 5;
			this->code = i;
			return (word.length());
		}
	}
}

void	Command::msgparser(const std::string input)
{
	// ConnectMess	type1;
	// ChannelMess	type2;
	// ServMess	type3;
	// UserMess	type4;
	// OperMess	type5;
	std::string	icomm = commandcheck(input);
	if (icomm.empty())
		this->params[0][0] = icomm;
	else
	{
		// size_t	len = this->idcomms(icomm);
		// for (size_t i = 0; input[len + i]; i++)
		// {
		// 	if (input[len + i] == ",")
		// }
	}
}
