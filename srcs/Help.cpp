/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 19:44:00 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/02 19:44:18 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

void Command::handleHELP(Client &sender, Server &server)
{
	(void) server;
	
	
	if (this->params.empty() || this->params[0][0].empty())
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + "* :** Help System **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + "* :Available commands: JOIN, PART, PING, NICK, TOPIC, INVITE, KICK, PRIVMSG, MODE, CAP, USER, PASS\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + "* :/HELP USERCMDS to list available\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + "* :commands, or join the #help channel\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	std::string cmd = this->params[0][0];
	if (cmd == "JOIN")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " JOIN :** The JOIN command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " JOIN :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " JOIN :The /JOIN command is used to join a specific channel on the IRC server.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " JOIN :Usage: /JOIN <channel>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "PART")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " PART :** The PART command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PART :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PART :The /PART command is used by a client to leave a specific channel on the IRC server. The client must provide the name of the channel they wish to leave. Once the client successfully parts from the channel, they will no longer receive messages from that channel and will be removed from the channel's member list.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " PART :Usage: /PART <channel>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "PING")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " PING :** The PING command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PING :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PING :The /PING command is used by a client to check the connectivity and responsiveness of the IRC server. When a client sends a PING command, the server responds with a PONG message, indicating that the server is alive and reachable. This command is often used to keep the connection between the client and server active and to detect any potential network issues.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " PING :Usage: /PING <server>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "NICK")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " NICK :** The NICK command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " NICK :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " NICK :The /NICK command is used by a client to set or change their nickname on the IRC server. The client must provide the desired nickname as a parameter. If the nickname is available and not already in use by another client, the server will update the client's nickname accordingly. If the nickname is already taken, the server will respond with an error message, prompting the client to choose a different nickname.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " NICK :Usage: /NICK <name>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "TOPIC")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " TOPIC :** The TOPIC command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " TOPIC :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " TOPIC :The /TOPIC command is used by a client to set or view the topic of a specific channel on the IRC server. To set the topic, the client must provide the channel name and the desired topic as parameters. If the client has the necessary permissions, the server will update the channel's topic accordingly. To view the current topic of a channel, the client can simply provide the channel name as a parameter, and the server will respond with the current topic of that channel.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " TOPIC :Usage: /TOPIC <channel> [topic]\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "INVITE")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " INVITE :** The INVITE command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " INVITE :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " INVITE :The /INVITE command is used by a client to invite another user to join a specific channel on the IRC server. The client must provide the nickname of the user they wish to invite and the name of the channel they want to invite them to. If the invited user accepts the invitation, they will be able to join the specified channel and participate in its discussions.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " INVITE :Usage: /INVITE <user> <channel>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "KICK")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " KICK :** The KICK command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " KICK :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " KICK :The /KICK command is used by a client to remove another user from a specific channel on the IRC server. The client must provide the nickname of the user they wish to kick and the name of the channel they want to kick them from. If the client has the necessary permissions, the server will remove the user from the channel and notify them of the action.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " KICK :Usage: /KICK <user> <channel>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "PRIVMSG")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " PRIVMSG :** The PRIVMSG command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PRIVMSG :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PRIVMSG :The /PRIVMSG command is the main way\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " PRIVMSG :to send messages to other users.\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "MODE")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " MODE :** The MODE command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " MODE :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " MODE :The /MODE command is used by a client to change or view the modes of a specific channel or user on the IRC server. To change modes, the client must provide the target (either a channel name or a user's nickname) and the mode changes as parameters. The mode changes can include adding or removing specific modes, such as setting a channel to invite-only or giving operator status to a user. To view the current modes of a channel or user, the client can simply provide the target as a parameter, and the server will respond with the current modes associated with that target.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " MODE :Usage: /MODE <target> [modes]\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "CAP")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " CAP :** The CAP command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " CAP :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " CAP :The /CAP command is used to negotiate capabilities between the client and server. It allows the client to request specific features or extensions that the server may support.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " CAP :Usage: /CAP <subcommand> [arguments]\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "USER")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " USER :** The USER command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " USER :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " USER :The /USER command is used by a client to specify their username, hostname, servername, and real name when connecting to the IRC server. It is typically sent after the NICK command and is required for successful registration with the server.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " USER :Usage: /USER <username> <hostname> <servername> <realname>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (cmd == "PASS")
	{
		std::string helpMsg = ":ircserver 704" + sender.getName() + " PASS :** The PASS command **\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PASS :\r\n";
		helpMsg += ":ircserver 705 " + sender.getName() + " PASS :The /PASS command is used by a client to provide a password for authentication when connecting to the IRC server. It is typically sent before the NICK and USER commands and is required if the server has password protection enabled.\r\n";
		helpMsg += ":ircserver 706 " + sender.getName() + " PASS :Usage: /PASS <password>\r\n";
		sender.appendBuffer(helpMsg);
		server.enablePollOut(sender.getFd());
		return;
	}
	else
	{
		std::string errcmd = ":ircserver 704 " + sender.getName() + "* :** Help System **\r\n";
		errcmd += ":ircserver 705 " + sender.getName() + "* :\r\n";
		errcmd += ":ircserver 705 " + sender.getName() + "* :I do not know anything about this.\r\n";
		errcmd += ":ircserver 705 " + sender.getName() + "* :Try /HELP USERCMDS to list available\r\n";
		errcmd += ":ircserver 706 " + sender.getName() + "* :commands, or join the #help channel\r\n";
		sender.appendBuffer(errcmd);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " requested help for unknown command: " << cmd << std::endl;
	}
}