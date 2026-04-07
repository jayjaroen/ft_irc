/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:03:17 by gyeepach          #+#    #+#             */
/*   Updated: 2026/04/07 21:45:42 by gyeepach         ###   ########.fr       */
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

enum ConnectMess
{
	CONNECT_UNKNOWN,
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


class Command
{
	private:
		// typedef void (Command::*Handler)(const IRCMessage&, Client&);
		// std::map<std::string, Handler> _hanlers;
		std::map<std::string, ConnectMess> _connectMap;
		std::map<std::string, ChannelMess> _channelMap;
		std::map<std::string, ServMess> _servMap;
		std::map<std::string, UserMess> _userMap;
		std::map<std::string, OperMess> _operMap;
		char	***params; //3 pointer for [array set #no][string #no][char(s) in string]
	public:
		Command();
		~Command();
		void initHandlers();
		void execute_command(const IRCMessage
		& msg);
		Command msgparser(const std::string input);
		void interpret_command(const IRCMessage &msg);
		// void handleChannelOp(ChannelMess cmd, const IRCMessage& msg);
		// void handleUserComm(UserMess cmd, const IRCMessage& msg);
		void handleConnection(ConnectMess cmd, const IRCMessage& msg);
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


enum ChannelMess
{
	CHANNEL_UNKNOWN,
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
	SERV_UNKOWN,
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
	USER_UNKOWN,
	PRIVMSG,
	NOTICE,
	WHO,
	WHOIS,
	WHOWAS
};

enum OperMess
{
	OPER_UNKOWN,
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