/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 21:11:06 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 21:11:36 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

static std::string buildClientPrefix(const Client &sender)
{
	std::string nickname = sender.getName();
	std::string username = sender.getUsername().empty() ? "unknown" : sender.getUsername();
	std::string host = sender.getIp().empty() ? "localhost" : sender.getIp();
	return ":" + nickname + "!" + username + "@" + host;
}

void Command::handleNick(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty() || this->params[0][0] == "")
	{
		std::string current_nick = sender.getName().empty() ? "unknown" : sender.getName();
		std::string err = ":ircserver " + intToString(ERR_NONICKNAMEGIVEN) + " " + current_nick + " :No nickname given\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	std::string newNick = this->params[0][0];

	while (!newNick.empty() && (newNick[newNick.length() - 1] == '\r' || newNick[newNick.length() - 1] == '\n' || newNick[newNick.length() - 1] == ' ')) {
		newNick.erase(newNick.length() - 1, 1);
	}

	if (server.findClient(newNick) != NULL && server.findClient(newNick)->getFd() != sender.getFd())
	{
		std::string current_nick = sender.getName().empty() ? "*" : sender.getName();
		if (current_nick.empty()) current_nick = "*";
		std::string err = ":ircserver " + intToString(ERR_NICKNAMEINUSE) + " " + current_nick + " " + newNick + " :Nickname is already in use\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return; 
	}

	if (newNick.empty() || newNick[0] == '#' || newNick[0] == '&' || newNick[0] == '!' || newNick[0] == '+'
		|| newNick[0] == '@' || newNick[0] == ':'  || newNick[0] == ' '
		|| newNick.find(' ') != std::string::npos || newNick.find('\t') != std::string::npos 
		|| newNick.find('\r') != std::string::npos || newNick.find('\n') != std::string::npos
		|| (newNick[0] >= '0' && newNick[0] <= '9'))
	{
		std::string err = ":ircserver " + intToString(ERR_ERRONEUSNICKNAME) + " " + newNick + " :Erroneous nickname\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}


	std::string oldNick = sender.getName();
	sender.setNick(newNick);
	sender.setNickSet(true);
	// std::cout << "Client FD " << sender.getFd() << " successfully set nick to " << newNick << std::endl;


	// if (sender.isAuthenticated() && !oldNick.empty() && oldNick != newNick)
	//     std::string nick_change_msg = ":" + oldNick + " NICK " + newNick + "\r\n";
	if (sender.isAuthenticated() && !oldNick.empty() && oldNick != newNick)
	{
		std::string nick_change_msg = buildClientPrefix(sender) + " NICK :" + newNick + "\r\n";
		sender.appendBuffer(nick_change_msg);
		server.enablePollOut(sender.getFd());
		
		std::vector<Channel*> my_channels = sender.getChannels();
		for (size_t i = 0; i < my_channels.size(); ++i)
		{
			my_channels[i]->broadcast(&sender, nick_change_msg);
		}
	}
}