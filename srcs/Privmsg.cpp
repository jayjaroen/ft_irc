/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:47:33 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:49:28 by gyeepach         ###   ########.fr       */
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

void Command::handlePRIVMSG(Server &server, Client &sender)
{
	//ERR 411
	if (this->params.empty() || this->params[0].empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NORECIPIENT) + " " + sender.getName() + " PRIVMSG :No recipient given\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	//ERR 412
	if (params.size() < 2 || params[1].empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NOTEXTTOSEND) + " " + sender.getName() + " PRIVMSG :No text to send\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	std::string target = this->params[0][0];
	std::string message = this->params[1][0]; // second parameter is the message
	//channel message
	// std::cout << "The channel name is " << target << std::endl;
	if (target[0] == '#' )
	{
		Channel* channel = server.findChannel(target);
		if (!channel) //403
		{
			// std::cout << "Channel not found" << std::endl;
			std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + target + " :No such channel\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			return;
		}
		if (!channel->hasClient(&sender)) //404
		{
			// std::cout << "Sender does not belong to " << target << std::endl;
			std::string err = ":ircserver " + intToString(ERR_CANNOTSENDTOCHAN) + " " + sender.getName() + " " + target + " :Cannot send to channel\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			return;
		}
		std::string text = buildClientPrefix(sender) + " PRIVMSG " + target + " : " + message + "\r\n";
		channel->broadcast(&sender, text);
	}
	else 
	{
		//user message
		Client *target_client = server.findClient(target);
		if (!target_client) //401
		{
			std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + target + " :No such nick/channel\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			return;
		}
		std::string text = buildClientPrefix(sender) + " PRIVMSG " + target + " : " + message + "\r\n";
		send(target_client->getFd(), text.c_str(), text.size(), 0);   
	}
}