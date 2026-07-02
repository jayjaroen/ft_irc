/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:37:14 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:38:18 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

static bool isValidJoinChannelName(const std::string &channel_name)
{
	if (channel_name.empty() || channel_name.length() > 50)
		return false;

	if (channel_name.length() == 1)
		return false;
	if (channel_name.empty() || channel_name.length() > 50)
		return false;
	if (channel_name[0] != '#')
		return false;
	if (channel_name.find(' ') != std::string::npos)
		return false;
	if (channel_name.find(',') != std::string::npos)
		return false;
	if (channel_name.find('\a') != std::string::npos)
		return false;
	if (channel_name.find('\r') != std::string::npos)
		return false;
	if (channel_name.find('\n') != std::string::npos)
		return false;
	return true;
}

static void sendJoinReplies(Client &sender, Channel *channel)
{
	if (channel == NULL)
		return;
	std::string channel_name = channel->getName();
	std::string join_prefix = buildClientPrefix(sender) + " JOIN :" + channel_name + "\r\n";
	sendResponse(sender.getFd(), join_prefix);
	channel->broadcast(&sender, join_prefix);
	std::string topic = channel->getTopic();
	if (!topic.empty())
	{
		std::string topic_msg = ":ircserver " + intToString(RPL_TOPIC) + " " + sender.getName() + " " + channel_name + " :" + topic + "\r\n";
		sendResponse(sender.getFd(), topic_msg);
		std::string setter = channel->getsetter_topic();
		std::string topic_time_msg = ":ircserver " + intToString(RPL_TOPICWHOTIME) + " " + sender.getName() + " " + channel_name + " " + setter + " :Topic set time\r\n";
		sendResponse(sender.getFd(), topic_time_msg);
	}
	std::string names_msg = ":ircserver " + intToString(RPL_NAMREPLY) + " " + sender.getName() + " = " + channel_name + " :";
	std::vector<Client*> clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (channel->isOperator(clients[i]->getFd()))
			names_msg += "@" + clients[i]->getName() + " ";
		else
			names_msg += clients[i]->getName() + " ";
	}
	names_msg += "\r\n";
	sendResponse(sender.getFd(), names_msg);
	std::string end_of_names_msg = ":ircserver " + intToString(RPL_ENDOFNAMES) + " " + sender.getName() + " " + channel_name + " :End of NAMES list\r\n";
	sendResponse(sender.getFd(), end_of_names_msg);
}

void Command::handleJOIN(Server &server, Client &sender)
{
	if (this->params.empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " JOIN :Not enough parameters\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (this->params[0].empty() || this->params.size() > 2)
	{
		std::string bad_channel = (this->params[0].empty() ? "" : this->params[0][0]);
		std::string err = ":ircserver " + intToString(ERR_BADCHANMASK) + " " + sender.getName() + " " + bad_channel + " :Bad channel mask\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		std::cerr << "Client FD " << sender.getFd() << " attempted to join with invalid JOIN parameters: " << bad_channel << std::endl;
		return;
	}
	std::vector<std::string> channels = this->params[0];
	std::vector<std::string> keys;
	if (this->params.size() > 1)
		keys = this->params[1];

	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string channel_name = channels[i];
		std::string key = "";
		if (i < keys.size())
			key = keys[i];

		if (!channel_name.empty() && (channel_name[0] == '&' || channel_name[0] == '+' || channel_name[0] == '!'))
		{
			std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channel_name + " :No such channel\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			continue;
		}
		if (!isValidJoinChannelName(channel_name))
		{
			std::string err = ":ircserver " + intToString(ERR_BADCHANMASK) + " " + sender.getName() + " " + channel_name + " :Bad channel mask\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			// std::cout << "Client FD " << sender.getFd() << " attempted to join channel with invalid name: " << channel_name << std::endl;
			continue;
		}
		Channel *channel = server.findChannel(channel_name);
		bool alreadyMember = (channel != NULL && channel->hasClient(&sender));

		if (!alreadyMember && sender.getNumChan() >= sender.getLimitChan())
		{
			std::string err = ":ircserver " + intToString(ERR_TOOMANYCHANNELS) + " " + sender.getName() + " " + channel_name + " :You have joined too many channels\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			continue;
		}

		if (channel != NULL && !alreadyMember)
		{
			if (channel->getKey() != "" && !channel->checkKey(key))
			{
				std::string err = ":ircserver " + intToString(ERR_BADCHANNELKEY) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+k)\r\n";
				sender.appendBuffer(err);
				server.enablePollOut(sender.getFd());
				// std::cout << "Client FD " << sender.getFd() << " attempted to join channel with incorrect key: " << channel_name << std::endl;
				continue;
			}
			if (channel->getLimit() > 0 && channel->getChannelSize() >= channel->getLimit())
			{
				std::string err = ":ircserver " + intToString(ERR_CHANNELISFULL) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+l)\r\n";
				sender.appendBuffer(err);
				server.enablePollOut(sender.getFd());
				// std::cout << "Client FD " << sender.getFd() << " attempted to join full channel: " << channel_name << std::endl;
				continue;
			}
			if (channel->isInviteOnly() && !channel->isInvited(&sender))
			{
				std::string err = ":ircserver " + intToString(ERR_INVITEONLYCHAN) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+i)\r\n";
				sender.appendBuffer(err);
				server.enablePollOut(sender.getFd());
				// std::cout << "Client FD " << sender.getFd() << " attempted to join invite-only channel without an invitation: " << channel_name << std::endl;
				continue;
			}
		}

		// std::cout << "Channel name is: " << "\"" << channel_name << "\"" << " key is " << "\"" << key << "\"" << std::endl;
		channel = server.findOrCreateChannel(channel_name, key, &sender);
		if (channel == NULL)
		{
			std::string err = ":ircserver " + intToString(ERR_BADCHANNELKEY) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+k)\r\n";
			sender.appendBuffer(err);
			server.enablePollOut(sender.getFd());
			continue;
		}
		sendJoinReplies(sender, channel);
	}
}