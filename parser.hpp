#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

#define SUCCESS 0
#define FAILURE 1

// struct IRCMessage{
// 	std::string prefix;
// 	std::string command;
// 	std::vector<std::string> params;
// 	std::string trailing;
// };

// enum tokentype
// {
// 	WORD,
// 	COLON,
// 	REFER,
// 	HASH
// };

enum CommandPrompts
{
	UNKNOWN_CMD,
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

// typedef struct tokens
// {
// 	tokentype		type;
// 	std::string		value;
// 	size_t			spaces;
// 	struct tokens	*next;
// };

class Command
{
	private:
		size_t		type;
		std::vector<std::vector<std::string> > params;
	public:
		Command();
		~Command();
		void		msgparser(const std::string input);
		std::string	commandcheck(const std::string input);
		size_t	lexer(const std::string &input, const std::string &icomm, std::vector<std::vector<std::string> > &output);
};


enum NumRpl
{
	NONEED,
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
	RPL_UMODEIS = 221,
	RPL_TRYAGAIN = 263,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_YOUREOPER = 381,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVILEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476, //optional
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOOPERHOST = 491,
	ERR_USERSDONTMATCH = 502,
//	.
//	.
//	.
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

#endif