/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:44:44 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/04 12:56:29 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

void Command::handleMODE(Client &sender, Server &server)
{
// MODE requires at least a target
if (this->params.size() < 1 || this->params[0].empty())
{
	std::string err = ":ircserver " +
	intToString(ERR_NEEDMOREPARAMS) + " " +
	sender.getName() +
	" MODE :Not enough parameters\r\n";
	
	sender.appendWriteBuffer(err);
	server.enablePollOut(sender.getFd());
	return;
}

std::string modeTarget = this->params[0][0];

// User mode handling not implemented
if (modeTarget.empty() || modeTarget[0] != '#')
	return;

Channel *channel = server.findChannel(modeTarget);
if (!channel)
{
	std::string err = ":ircserver " +
	intToString(ERR_NOSUCHCHANNEL) + " " +
	sender.getName() + " " +
	modeTarget +
	" :No such channel\r\n";
	
	sender.appendWriteBuffer(err);
	server.enablePollOut(sender.getFd());
	return;
}

// MODE #channel
if (this->params.size() < 2 || this->params[1].empty())
{
	std::string servername = "ircserver";
	std::string nick = sender.getName();
	std::string modestring = "+";
	std::string mode_args = "";
	
	if (channel->isInviteOnly())
		modestring += "i";
	if (channel->getKey() != "")
		modestring += "k";
	if (channel->getLimit() > 0)
		modestring += "l";
	if (channel->getTopic_mode())
		modestring += "t";
	std::string rpl_mode_324 = ":ircserver " + intToString(RPL_CHANNELMODEIS) + " " + nick + " " + channel->getName() + " : " + modestring + mode_args + "\r\n";
	sender.appendWriteBuffer(rpl_mode_324);
	server.enablePollOut(sender.getFd());
	std::stringstream ss_chan;
	ss_chan << channel->getCreationTime();
	std::string rpl_329_msg = ":ircserver " + intToString(RPL_CREATIONTIME) + " " + sender.getName() + " " + channel->getName() + " " + ss_chan.str() + "\r\n";
	sender.appendWriteBuffer(rpl_329_msg);
	server.enablePollOut(sender.getFd());
	return;
}
if (channel->isOperator(sender.getFd()) == false)
{
	std::string err = ":ircserver " +
		intToString(ERR_CHANOPRIVSNEEDED) + " " +
		sender.getName() + " " +
		modeTarget +
		" :You're not channel operator\r\n";

	sender.appendWriteBuffer(err);
	server.enablePollOut(sender.getFd());
	return;
	}

	std::string modeChanges = this->params[1][0];

	// if (modeChanges.empty())
	// {
	// 	std::string err = ":ircserver " +
	// 		intToString(ERR_NEEDMOREPARAMS) + " " +
	// 		sender.getName() +
	// 		" MODE :Not enough parameters\r\n";

	// 	sender.appendWriteBuffer(err);
	// 	server.enablePollOut(sender.getFd());
	// 	return;
	// }

	if (modeChanges[0] != '+' && modeChanges[0] != '-')
	{
		std::string err = ":ircserver " +
			intToString(ERR_UMODEUNKOWNFLAG) + " " +
			sender.getName() + " " +
			modeChanges +
			" :Unknown MODE flag\r\n";

		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}

	// Need at least "+i", "-k", etc.
	if (modeChanges.length() != 2)
	{
		std::string err = ":ircserver " +
			intToString(ERR_UMODEUNKOWNFLAG) + " " +
			sender.getName() +
			" :Unknown MODE flag\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// sender.appendWriteBuffer(err);
		// server.enablePollOut(sender.getFd());
		return;
	}

	switch (modeChanges[1])
	{
		case 'k':
		{
			if (modeChanges[0] == '+')
			{
				if ((this->params.size() < 3 || this->params[2].empty()))
				{
					std::string err = ":ircserver " +
						intToString(ERR_NEEDMOREPARAMS) + " " +
						sender.getName() +
						" MODE :Not enough parameters\r\n";

					sender.appendWriteBuffer(err);
					server.enablePollOut(sender.getFd());
					return;
				}
			}
			channel->handleKeyMode(
				sender,
				modeChanges,
				this->params[2][0],
				server);
			break;
		}

		case 'l':
		{
			std::string limitParam;
			if (modeChanges[0] == '+')
			{
				if (this->params.size() < 3 || this->params[2].empty())
				{
					std::string err = ":ircserver " +
						intToString(ERR_NEEDMOREPARAMS) + " " +
						sender.getName() +
						" MODE :Not enough parameters\r\n";
					sender.appendWriteBuffer(err);
					server.enablePollOut(sender.getFd());
					return;
				}
				limitParam = this->params[2][0];
				if (isnumeric(limitParam) == false)
				{
					return;
				}
			}
			else if (modeChanges[0] == '-' && this->params.size() < 2)
			{
				std::string err = ":ircserver " +
					intToString(ERR_NEEDMOREPARAMS) + " " +
					sender.getName() +
					" MODE :Not enough parameters\r\n";
				sender.appendWriteBuffer(err);
				server.enablePollOut(sender.getFd());
				return;
			}
			channel->handleLimitMode(
				sender,
				modeChanges,
				limitParam,
				server);
			break;
		}

		case 'o':
		{
			if (this->params.size() < 3 || this->params[2].empty())
			{
				std::string err = ":ircserver " +
					intToString(ERR_NEEDMOREPARAMS) + " " +
					sender.getName() +
					" MODE :Not enough parameters\r\n";
				sender.appendWriteBuffer(err);
				server.enablePollOut(sender.getFd());
				return;
			}
			channel->handleOperatorMode(
				sender,
				modeChanges,
				this->params[2][0],
				server);
			break;
		}

		case 't':
			channel->handleTopicMode(sender, modeChanges, server);
			break;

		case 'i':
			channel->handleInviteMode(sender, modeChanges, server);
			// server.set_check_broadcast(true);
			break;

		default:
		{
			std::string err = ":ircserver " +
				intToString(ERR_UMODEUNKOWNFLAG) + " " +
				sender.getName() + " " +
				std::string(1, modeChanges[1]) +
				" :is unknown mode char\r\n";

			sender.appendWriteBuffer(err);
			server.enablePollOut(sender.getFd());
			return;
		}
	}
}