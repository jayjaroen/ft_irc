/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:03:17 by gyeepach          #+#    #+#             */
/*   Updated: 2026/05/02 15:18:05 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <cctype>
#include <sstream>

#include "./include/Client.hpp"
#include "./include/Server.hpp"

class Server;
class Client;

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
	USER,
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
	RESTART,
	PONG
};


class Command
{
	private:
		size_t		type;
		std::vector<std::vector<std::string> > params;
	public:
		Command();
		~Command();
		// void intitHandlers();
		// void execute_command(Client &sender);
		void execute_command(Server &server, Client &sender);
		void		msgparser(const std::string input);
		std::string	commandcheck(const std::string input);
		size_t	lexer(const std::string& input, const std::string& icomm, std::vector<std::vector<std::string> >& output);
		void handleNick(Client &sender);
		void handleUSER(Client &sender);
			// void handleUSER(Client &sender); --- IGNORE ---
				// I have a question about this function should I pass the server as a parameter to handleUSER function or not??
				// No, you do not need to pass the server as a parameter to the handleUSER function. The USER command is typically used for setting the username and other user-related information, and it does not require direct access to the server's state or configuration. The handleUSER function can simply update the client's information based on the parameters provided in the command without needing to interact with the server. Therefore, it is sufficient to only pass the Client object as a parameter to the handleUSER function.
		void handlePass(Client &sender, Server &server);
			// void handlePass(Client &sender); --- IGNORE ---
			// void handlePass(Client &sender, Server &server); --- IGNORE ---
				// I have a question about this function should I pass the server as a parameter to handlePass function or not??
				// Yes, you should pass the server as a parameter to the handlePass function. The PASS command is typically used for authentication, and it may require access to the server's state or configuration to validate the password and authenticate the client. By passing the server as a parameter, you can access any necessary information or functions within the server class to properly handle the PASS command and authenticate the client.
		void handleQuit(Client &sender, Server &server);
		// void handlePRIVMSG(Client &sender);
		void handleJOIN(Server &server, Client &sender);
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
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476, //optional
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOOPERHOST = 491,
	ERR_USERSDONTMATCH = 502,
	ERR_ALREADYREGISTRED = 462,
	ERR_NOTREGISTERED = 451
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

std::string intToString(int n);

#endif