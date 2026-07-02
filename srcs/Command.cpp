/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/07/02 23:42:54 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <string>

// void sendResponse(int fd, const std::string &message)
// {
// 	if (fd <= 0)
// 		return;
//     // std::cout << "Sending to FD " << fd << ": " << message << std::endl;
// 	ssize_t sent = send(fd, message.c_str(), message.length(), MSG_NOSIGNAL);
// 	if (sent < 0)
// 	{
// 		::perror("Failed to send message");
// 	}
// }

void Command::execute_command(Server &server, Client &sender)
{
	size_t cmdType = this->type;

	if (cmdType == QUIT)
	{
		handleQuit(sender, server);
		return;
	}
	if (sender.isAuthenticated() == false)
	{
		if (cmdType != PASS && cmdType != NICK && cmdType != USER && cmdType != CAP)
		{
			std::string err = ":ircserver " + intToString(ERR_NOTREGISTERED) + " " + sender.getName() + " :You have not registered\r\n";
			sender.appendWriteBuffer(err);
			server.enablePollOut(sender.getFd());
			// std::cout << "Client fd " << sender.getFd() << " attempted to execute command without registering: " << err << std::endl;
			return ;
		}
	}
	switch (cmdType)
	{
		case CAP:     handleCAP(sender, server);     break;
		case USER:    handleUSER(sender, server);    break;
		case PASS:    handlePass(sender, server);    break;
		case NICK:    handleNick(sender, server);    break;
		case JOIN:    handleJOIN(server, sender);    break;
		case PRIVMSG: handlePRIVMSG(server, sender); break;
		case QUIT:    handleQuit(sender, server);    return;
		case MODE:    handleMODE(sender, server);    break;
		case HELP:    handleHELP(sender, server);    break;
		case PART:    handlePart(server, sender);    break;
		case INVITE:  handleINVITE(sender, server);  break;
		case KICK:    handleKICK(sender, server);    break;
		case TOPIC:   handleTOPIC(sender, server);   break;
		case PING:
		{
			if (this->params.empty() || this->params[0].empty()) return;
			std::string token = this->params[0][0];
			std::string pongResponse = ":ircserver PONG ircserver :" + token + "\r\n";
			sender.appendWriteBuffer(pongResponse);
			server.enablePollOut(sender.getFd());
			break;
		}
		default:      break;
	}
	if (sender.isPassSet() == true && sender.isNickSet() == true && sender.isUserSet() == true && sender.isAuthenticated() == false)
	{
		sendWelcomeMessage(server, sender);
	}
	if (this->type == 0)
		return;                                                                                                                                         
}



void sendWelcomeMessage(Server &server, Client &sender)
{
	if (sender.isNickSet() && sender.isUserSet() && sender.isPassSet() && !sender.isAuthenticated())
	{
		sender.setAuthenticated(true);
		// std::cout << "Client FD " << sender.getFd() << " has successfully registered." << std::endl;
		std::string clientNick = sender.getName();
		std::string welcomeMsg = ":ircserver 001 " + clientNick + " :Welcome to the IRC server, " + clientNick + "!\r\n";
		welcomeMsg += ":ircserver 002 " + clientNick + " :Your host is ircserver, running version 1.0\r\n";
		welcomeMsg += ":ircserver 003 " + clientNick + " :This server was created on " + server.get_creation_date() + "\r\n";
		welcomeMsg += ":ircserver 004 " + clientNick + " ircserver 1.0 i tkolk\r\n";
		
		sender.appendWriteBuffer(welcomeMsg);
		server.enablePollOut(sender.getFd());
	}
}

bool isnumeric(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}