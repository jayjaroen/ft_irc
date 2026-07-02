/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:49:11 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:49:42 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

void Command::handleUSER(Client &sender, Server &server)
{
	(void) server;
	const size_t userlen = 9;
	if (this->params.empty() || this->params.size() != 4 || this->params[0].empty() || this->params[1].empty() || this->params[2].empty() || this->params[3].empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " USER :Not enough parameters\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (sender.isAuthenticated() || sender.isUserSet())
	{
		std::string err = ":ircserver " + intToString(ERR_ALREADYREGISTERED) + " " + sender.getName() + " :You may not register again\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to re-register." << std::endl;
		return;
	}
	std::string username = this->params[0][0];
	std::string hostname = this->params[1][0];
	std::string servername = this->params[2][0];
	std::string realname = this->params[3][0];
	if (username.length() > userlen)
		username = username.substr(0, userlen);
	if (hostname.length() > 63)
		hostname = hostname.substr(0, 63);
	if (servername.length() > 63)
		servername = servername.substr(0, 63);
	if (realname.length() > 128)
		realname = realname.substr(0, 128);
	sender.setUsername(username);
	// std::cout << "Client FD " << sender.getFd() << " set username to " << username << std::endl;
	sender.setUserSet(true);
}