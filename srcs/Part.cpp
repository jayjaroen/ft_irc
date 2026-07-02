/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:45:22 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:47:54 by gyeepach         ###   ########.fr       */
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

void    Command::handlePart(Server &server, Client &sender)
{
	if (this->params.empty() || this->params[0].empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() +
			"PART :Not enough parameters\r\n";
		 
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	std::string channel_name = this->params[0][0];
	Channel* channel = server.findChannel(channel_name);
	if (!channel)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channel_name + " :No such channel\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (!channel->hasClient(&sender))
	{
		std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + channel_name + " :You're not on that channel\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client does not belong to the channel " << channel_name << std::endl;
		return;
	}
	std::string message = buildClientPrefix(sender) + " PART " + channel_name + "\r\n";
	channel->broadcast(&sender, message);
	sender.appendBuffer(message);
	server.enablePollOut(sender.getFd());
	channel->removeClient(&sender);
	channel->removeOperator(&sender);
	// std::cout << sender.getName() << " left channel " << channel_name << std::endl;
	if (channel->get_operators_size() == 0
		&& channel->getChannelSize() > 0)
	{
		Client* new_admin = channel->getClients()[0];
		std::string new_name = "@" + new_admin->getName();
		// channel->setAdmin(new_admin);
		channel->addOperator(new_admin->getFd());
		std::string admin_msg = ":ircserver " + intToString(RPL_YOUREOPER) + " " + new_admin->getName() + " :You are now the channel operator\r\n";
		sender.appendBuffer(admin_msg);
		server.enablePollOut(sender.getFd());
		channel->broadcastModeChange(*new_admin, "+o " + new_admin->getName());
		// std::cout << "New admin of channel " << channel_name << " is " << new_admin->getName() << std::endl;
	}
	// std::cout << sender.getName() << " left channel " << channel_name << std::endl;
	if (channel->isEmpty())
	{
		// std::cout << "Deleting the channel " << channel_name << std::endl;
		server.deleteChannel(channel_name);
	}
}