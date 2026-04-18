#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

struct IRCMessage{
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
	std::string trailing;
};

class Command
{
	private:
		// std::string	command;
		size_t		type;
		// typedef void (Command::*Handler)(const IRCMessage&, Client&);
		// std::map<std::string, Handler> _hanlers;
		std::string	**params; //2 pointer for [array set #no][string #no]
	public:
		Command();
		~Command();
		// void intitHandlers();
		// void execute_comand(const IRCMessage & msg);
		void		msgparser(const std::string input);
		std::string	commandcheck(const std::string input);
		// void handleChannelOp(ChannelMess cmd, const IRCMessage& msg);
		// void handleUserComm(UserMess cmd, const IRCMessage& msg);
		// void handleConnection(ConnectMess cmd, const IRCMessage& msg);
		// void sendResponse(int fd, const std::string& response);
};

// class EnviVar
// {
// 	private:
// 		char	**envmod;
// 		EnviVar();
// 	public:
// 		~EnviVar();
// 		char	**envparser(const char **env);
// };

// enum ConnectMess
// {
// 	CAP,
// 	AUTHENTICATE,
// 	PASS,
// 	NICK,
// 	USER,
// 	PING,
// 	PONG,
// 	OPER,
// 	QUIT,
// 	ERROR
// };
// enum ChannelMess
// {
// 	JOIN,
// 	PART,
// 	TOPIC,
// 	NAMES,
// 	LIST,
// 	INVITE,
// 	KICK
// };
// enum ServMess
// {
// 	MOTD,
// 	VERSION,
// 	ADMIN,
// 	CONNECT,
// 	LUSERS,
// 	TIME,
// 	STATS,
// 	HELP,
// 	INFO,
// 	MODE
// };
// enum UserMess
// {
// 	PRIVMSG,
// 	NOTICE,
// 	WHO,
// 	WHOIS,
// 	WHOWAS
// };
// enum OperMess
// {
// 	KILL,
// 	REHASH,
// 	RESTART,
// 	SQUIT
// };

enum CommandPrompts
{
	CAP,
	CONNECT,
	QUIT,
	JOIN,
	PART,
	PING,
	HELP,
	NICK,
	PASS,
	TOPIC,
	INVITE,
	KICK,
	PRIVMSG,
	OPER,
	MODE,
	RESTART
};

// enum NumRpl
// {
// 	UNKNOWN_CMD,
// 	RPL_WELCOME,
// 	.
// 	.
// 	.
// };


// IRCMessage translateFromParser(char ***params);

#endif