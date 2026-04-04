/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:03:17 by gyeepach          #+#    #+#             */
/*   Updated: 2026/04/04 00:28:21 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		// enum	ConnectMess;
		// enum	ChannelMess;
		// enum	ServMess;
		// enum	UserMess;
		// enum	OperMess;
		// typedef void (Command::*Handler)(const IRCMessage&, Client&);
		// std::map<std::string, Handler> _hanlers;
		char	***params; //3 pointer for [array set #no][string #no][char(s) in string]
		Command();
	public:
		~Command();
		void intitHandlers();
		void execute_comand(const IRCMessage
		& msg);
		Command msgparser(const std::string input);
		// void handleChannelOp(ChannelMess cmd, const IRCMessage& msg);
		// void handleUserComm(UserMess cmd, const IRCMessage& msg);
		// void handleConnection(ConnectMess cmd, const IRCMessage& msg);
		void sendResponse(int fd, const std::string& response);
};

class EnviVar
{
	private:
		char	**envmod;
		EnviVar();
	public:
		~EnviVar();
		char	**envparser(const char **env);
};

enum ConnectMess
{
	UNKNOWN_CMD,
	CAP,
	AUTHENTICATE,
	PASS,
	NICK,
	USER,
	PING,
	PONG,
	OPER,
	QUIT,
	ERROR
};

enum ChannelMess
{
	Channel_unkown,
	JOIN,
	PART,
	TOPIC,
	NAMES,
	LIST,
	INVITE,
	KICK
};

enum ServMess
{
	Serv_unkown,
	MOTD,
	VERSION,
	ADMIN,
	CONNECT,
	LUSERS,
	TIME,
	STATS,
	HELP,
	INFO,
	MODE
};

enum UserMess
{
	User_unkown,
	PRIVMSG,
	NOTICE,
	WHO,
	WHOIS,
	WHOWAS
};

enum OperMess
{
	Oper_unkown,
	KILL,
	REHASH,
	RESTART,
	SQUIT
};

// enum NumRpl
// {
// 	UNKNOWN_CMD,
// 	RPL_WELCOME,
// 	.
// 	.
// 	.
// };


IRCMessage translateFromParser(char ***params);

#endif