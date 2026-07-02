/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:38:57 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:40:02 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

void Command::handleINVITE(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty() || this->params.size() < 2 || this->params[1].empty())
	{
		std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " INVITE :Not enough parameters\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to invite without providing necessary parameters." << std::endl;        
		return;
	}
	std::string targetNick = this->params[0][0];
	std::string channelName = this->params[1][0];
	Client* targetClient = server.findClient(targetNick);
	Channel* targetChannel = server.findChannel(channelName);
	if (!targetClient)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + targetNick + " :No such nick\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to invite non-existent user: " << targetNick << std::endl;
		return;
	}
	if (!targetChannel)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " :No such channel\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to invite to non-existent channel: " << channelName << std::endl;
		return;
	}
	if (!targetChannel->hasClient(&sender))
	{
		std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to invite user to channel they are not part of: " << channelName << std::endl;
		return;
	}
	if (targetChannel->isInviteOnly() && !targetChannel->isOperator(sender.getFd()))
	{
		std::string err = ":ircserver " + intToString(ERR_CHANOPRIVSNEEDED) + " " + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (targetChannel->hasClient(targetClient))
	{
		std::string err = ":ircserver " + intToString(ERR_USERONCHANNEL) + " " + sender.getName() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to invite user who is already on the channel: " << targetNick << " to channel: " << channelName << std::endl;
		return;
	}
	if (targetChannel->isInviteOnly())
		targetChannel->addInviteClient(targetClient);
	
	std::string inviteMsg = ":ircserver " + intToString(RPL_INVITING) + " " + sender.getName() + " " + targetNick + " :" + channelName + "\r\n";
	// targetChannel->removeClient(targetClient); // remove the client from the channel's invite list if they were previously invited
	// sendResponse(targetClient->getFd(), inviteMsg);
	sender.appendBuffer(inviteMsg);
	server.enablePollOut(targetClient->getFd());
	// std::cout << sender.getName() << " invited " << targetNick << " to join channel " << channelName << std::endl;
}