/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:03:17 by gyeepach          #+#    #+#             */
/*   Updated: 2026/04/11 14:30:42 by gyeepach         ###   ########.fr       */
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
#include <algorithm>

struct IRCMessage{
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
	std::string trailing;
};

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

class Command
{
	private:
		// typedef void (Command::*Handler)(const IRCMessage&, Client&);
		// std::map<std::string, Handler> _hanlers;
		std::map<std::string, CommandPrompts> _commandprompts;
		char	***params; //3 pointer for [array set #no][string #no][char(s) in string]
	public:
		Command();
		~Command();
		void initHandlers();
		void execute_command(IRCMessage &msg, Client &sender);
		// Command msgparser(const std::string input);
		void convert_to_upper(IRCMessage &msg);
		void handleNick(IRCMessage &msg, Client &sender);
		// void handleChannelOp(ChannelMess cmd, const IRCMessage& msg);
		// void handleUserComm(UserMess cmd, const IRCMessage& msg);
		// void handleConnection(ConnectMess cmd, const IRCMessage& msg);
		// void sendResponse(int fd, const std::string& response);
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