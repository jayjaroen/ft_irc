/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:50:32 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 23:38:47 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

void Command::handleQuit(Client &sender, Server &server)
{
	sender.appendWriteBuffer("Goodbye!\r\n");
	server.enablePollOut(sender.getFd());
	server.disconnectClient(sender.getFd());
}