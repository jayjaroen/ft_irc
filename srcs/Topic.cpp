/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:43:08 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/04 10:25:12 by gyeepach         ###   ########.fr       */
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

void Command::handleTOPIC(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty())
	{
		std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " TOPIC :Not enough parameters\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	std::string channelName = this->params[0][0];
	Channel *target_channel = server.findChannel(channelName);

	if (target_channel == NULL)
	{
		std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " :No such channel\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	if (!target_channel->hasClient(&sender))
	{
		std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	if (this->params.size() < 2 || this->params[1].empty())
	{
		std::string topic = target_channel->getTopic();
		if (!topic.empty())
		{
			std::string topicMsg = ":ircserver " + intToString(RPL_TOPIC) + " " + sender.getName() + " " + channelName + " :" + topic + "\r\n";
			sender.appendWriteBuffer(topicMsg);
			server.enablePollOut(sender.getFd());

			std::stringstream ss;
			ss << target_channel->getCreationTime_Topic();
			

			std::string topic_time_msg = ":ircserver " + intToString(RPL_TOPICWHOTIME) + " " + sender.getName() + " " + channelName + " " + target_channel->getsetter_topic() + " " + ss.str() + "\r\n";
			sender.appendWriteBuffer(topic_time_msg);
			server.enablePollOut(sender.getFd());
		}
		else
		{
			std::string noTopicMsg = ":ircserver " + intToString(RPL_NOTOPIC) + " " + sender.getName() + " " + channelName + " :No topic is set\r\n";
			sender.appendWriteBuffer(noTopicMsg);
			server.enablePollOut(sender.getFd());
		}
		return;
	}

	std::string newTopic = this->params[1][0];
	bool mode = target_channel->getTopic_mode();

	if (mode == true && target_channel->isOperator(sender.getFd()) == false)
	{
		std::string err = ":ircserver " + intToString(ERR_CHANOPRIVSNEEDED) + " " + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	target_channel->setTopic(newTopic, sender.getName());

	std::stringstream ss;
	ss << target_channel->getCreationTime_Topic();
	std::string time_str = ss.str();

	std::string topic_broadcast = buildClientPrefix(sender) + " TOPIC " + channelName + " :" + newTopic + "\r\n";
	std::string time_broadcast  = ":ircserver " + intToString(RPL_TOPICWHOTIME) + " " + sender.getName() + " " + channelName + " " + sender.getName() + " " + time_str + "\r\n";

	sender.appendWriteBuffer(topic_broadcast);
	sender.appendWriteBuffer(time_broadcast);
	server.enablePollOut(sender.getFd());
	// target_channel->broadcast(&sender, topic_broadcast);
	// target_channel->broadcast(&sender, time_broadcast);
}