/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:41:02 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 23:38:01 by gyeepach         ###   ########.fr       */
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

void Command::handleKICK(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty() || this->params.size() < 2 || this->params[1].empty())
	{
		std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " KICK :Not enough parameters\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick without providing necessary parameters." << std::endl;        
		return;
	}
	std::string channelName = this->params[0][0];
	std::string targetNick = this->params[1][0];
	// check error ERR_BADCHANMASK
	if (channelName[0] != '#' && channelName[0] != '&') // check if the channel name starts with # or & which are valid channel prefixes in IRC
	{
		std::string err = ":ircserver " + intToString(ERR_BADCHANMASK) + " " + sender.getName() + " KICK :Bad channel mask\r\n";    
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick with invalid parameters: targetNick: " << targetNick << ", channelName: " << channelName << std::endl;
		return;
	}
	Client* targetClient = server.findClient(targetNick);
	Channel* targetChannel = server.findChannel(channelName);
	// Check first if Channel exit
	if (!targetChannel)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " : no such channel\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (!targetClient)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + targetNick + " :No such nick\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick non-existent user: " << targetNick << std::endl;
		return;
	}
	if (!targetChannel->hasClient(&sender))
	{
		std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick user from channel they are not part of: " << channelName << std::endl;
		return;
	}
	if (!targetChannel->hasClient(targetClient))
	{
		std::string err = ":ircserver " + intToString(ERR_USERNOTINCHANNEL) + " " + sender.getName() + " " + targetNick + " " + channelName + " :is not on channel\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick user who is not on the channel: " << targetNick << " from channel: " << channelName << std::endl;
		return;
	}
	if (targetChannel->isOperator(sender.getFd()) == false)
	{
		std::string err = ":ircserver " + intToString(ERR_CHANOPRIVSNEEDED) + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to kick user from channel without operator privileges: " << channelName << std::endl;
		return;
	}
	// std::string kickMsg = ":ircserver KICK " + channelName + " " + targetNick + " :Kicked by " + sender.getName() + "\r\n";
	// targetChannel->broadcast(targetClient, kickMsg); // Notify the channel that the user has been kicked
	// targetChannel->removeClient(targetClient); // Remove the client from the channel
	// std::cout << sender.getName() << " kicked " << targetNick << " from channel " << channelName << std::endl;
	// if (targetChannel->isEmpty())
	// {
	//     std::cout << "Deleting the channel " << channelName << " as it is now empty after kick." << std::endl;
	//     server.deleteChannel(channelName);
	// }
	// ... โค้ดดักเช็ค Operator และดักเช็ค NULL ด้านบนคงเดิมไว้ทั้งหมด ...

	std::string kickMsg = buildClientPrefix(sender) + " KICK " + channelName + " " + targetNick + " :Kicked by " + sender.getName() + "\r\n";
	

	// sendResponse(targetClient->getFd(), kickMsg);
	sender.appendWriteBuffer(kickMsg);
	server.enablePollOut(targetClient->getFd());
	targetChannel->broadcast(&sender, kickMsg); 


	targetChannel->removeClient(targetClient);
	targetChannel->removeOperator(targetClient);
	
	// std::cout << sender.getName() << " kicked " << targetNick << " from channel " << channelName << std::endl;
	

	if (targetChannel->isEmpty())
	{
		// std::cout << "Deleting the channel " << channelName << " as it is now empty after kick." << std::endl;
		server.deleteChannel(channelName);
		return;
	}
}