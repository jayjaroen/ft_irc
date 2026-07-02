/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:42:37 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 23:35:22 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

void Command::handleCAP(Client &sender, Server &server)
{
	(void) server;
	if (this->params.empty())
	{
		std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " CAP :Not enough parameters\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to inquire capabilities without proper input(s)." << std::endl;
		return;
	}
	std::string subcommand = this->params[0][0];
	std::string commlist[] = {"LS", "LIST", "REQ", "END"};
	CapSubCommands  sub = UNKNOWN_CMD_CAP;
	for (size_t i = 0; i < 4; i++)
	{
		if (this->params[0][0] == commlist[i])
		{
			switch (i)
			{
				case 0:
					sub = LS;
					break;
				case 1:
					sub = LIST;
					break;
				case 2:
					sub = REQ;
					break;
				case 3:
					sub = END;
					break;
				
				default:
					break;
			}
		}
	}
	if (sub == UNKNOWN_CMD_CAP)
	{
		std::string err = ":ircserver "+ intToString(ERR_INVALIDCAPCMD) + " " + sender.getName() + " CAP :Invalid CAP subcommands\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to inquire capabilities without proper subcommand." << std::endl;
		return;
	}
	switch (sub)
	{
		case LS:
			sender.setCapNegotiating(true); // set a flag in client to indicate that CAP negotiation is in progress
			if (this->params.size() > 1 && !this->params[1].empty()) // leak solved checked
			{
				sender.appendWriteBuffer(":ircserver CAP " + sender.getName() +  " LS "  + this->params[1][0] + " " + " multi-prefix\r\n");
				server.enablePollOut(sender.getFd());
			}
			else
			{
				sender.appendWriteBuffer(":ircserver CAP " + sender.getName() + " LS multi-prefix\r\n");
				server.enablePollOut(sender.getFd());
			}
			break;
		case REQ:
		{
			bool has_multi_prefix = false;
			for (size_t i = 1; i < this->params.size(); ++i)
			{
				if (!this->params[i].empty())
				{
					if (this->params[i][0].find("multi-prefix") != std::string::npos)
					{
						has_multi_prefix = true;
						break;
					}
				}
			}
			if (has_multi_prefix)
			{
				std::string reply = ":ircserver CAP " + sender.getName() + " ACK :multi-prefix\r\n";
				sender.appendWriteBuffer(reply);
				server.enablePollOut(sender.getFd());
			}
			else
			{
				std::string requested = (this->params.size() > 1 && !this->params[1].empty()) ? this->params[1][0] : "unknown";
				std::string reply = ":ircserver CAP " + sender.getName() + " NAK :" + requested + "\r\n";
				sender.appendWriteBuffer(reply);
				server.enablePollOut(sender.getFd());
			}
			break;
		}
		case END:
			sender.setCap(true); // set a flag in client to indicate that CAP negotiation is complete and the client can now use the enabled capabilities
			sender.setCapNegotiating(false); // reset the negotiating flag
			// sendWelcomeMessage(server, sender);
			// std::cout << "Client FD " << sender.getFd() << "end CAP negotiations" << std::endl;
			break;
		
		default:
			break;
	}
}