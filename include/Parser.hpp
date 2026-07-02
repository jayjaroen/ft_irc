/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:03:17 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 14:41:18 by gyeepach         ###   ########.fr       */
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
#include "./include/Channel.hpp"

class Server;
class Client;

#define SUCCESS 0
#define FAILURE 1

enum CommandPrompts
{
	UNKNOWN_CMD,
	CAP,
	USER, // 1
	QUIT,
	JOIN,
	PART,
	PING,
	HELP,
	NICK, // 1
	PASS, // 1
	TOPIC,
	INVITE,
	KICK,
	PRIVMSG,
	OPER,
	MODE,
	RESTART,
	PONG
};

enum CapSubCommands
{
	UNKNOWN_CMD_CAP,
	LS,
	LIST,
	REQ,
	END
};

class Command
{
	private:
		size_t		type;
		std::vector<std::vector<std::string> > params;
	public:
		Command();
		~Command();
		void	execute_command(Server &server, Client &sender);
		void	msgparser(const std::string input);
		std::string	commandcheck(const std::string input);
		size_t	lexer(const std::string& input, const std::string& icomm, std::vector<std::vector<std::string> >& output);
		void	handleNick(Client &sender, Server &server);
		void	handleUSER(Client &sender, Server &server);
		void	handlePass(Client &sender, Server &server);
		void	handleQuit(Client &sender, Server &server);
		void	handlePRIVMSG(Server &server, Client &sender);
		void	handleJOIN(Server &server, Client &sender);
		void	handlePart(Server &server, Client &sender);
		void	handleMODE(Client &sender, Server &server);
		void	handleHELP(Client &sender, Server &server);
		void	handleCAP(Client &sender, Server &server);
		void	handleINVITE(Client &sender, Server &server);
		void	handleKICK(Client &sender, Server &server);
		void	handleTOPIC(Client &sender, Server &server);
};


enum NumRpl
{
	NONEED,
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_YOUREOPER = 381,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_INVALIDCAPCMD = 410,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_USERSDONTMATCH = 502,
	ERR_NOTREGISTERED = 451,
	ERR_UMODEUNKOWNFLAG = 221,
	RPL_CHANNELMODEIS = 324,
	RPL_CREATIONTIME = 329
};


std::string intToString(int n);
void sendWelcomeMessage(Server &server, Client &sender);
bool isnumeric(const std::string& str);

#endif