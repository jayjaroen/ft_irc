/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:51:06 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:51:20 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

void Command::handlePass(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty())
		return;
	if (sender.isAuthenticated())
	{
		std::string err = ":ircserver " + intToString(ERR_ALREADYREGISTERED) + " " + sender.getName() + " :You may not register again\r\n";
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to re-authenticate." << std::endl;
	}
	else if (this->params[0][0] == server.get_password())
	{
		sender.setPassSet(true);
		// std::cout << "Client FD " << sender.getFd() << " authenticated successfully." << std::endl;
	}
	else
	{
		std::string err = ":ircserver " + intToString(ERR_PASSWDMISMATCH) + " " + sender.getName() + " :Password incorrect\r\n";  
		sender.appendBuffer(err);
		server.enablePollOut(sender.getFd()); 
		sender.setPassSet(false);
	}
}