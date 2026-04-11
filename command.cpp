/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/11 14:23:10 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

IRCMessage translateFromParser(char ***params)
{
    IRCMessage msg;

    if (!params)
        return msg;
    
    if (params[0] && *(params[0]))
        msg.prefix = *(params[0]);

    if (params[1] && *(params[1]))
        msg.command = *(params[1]);
    
    for (int i = 2; params[i] != NULL; ++i)
    {
        if (*(params[i]))
            msg.params.push_back(*(params[i]));
    }
    return msg;
}



void Command::interpret_command(const IRCMessage &msg)
{
    std::string cmdUpper = msg.command;
    std::transform(cmdUpper.begin(), cmdUpper.end(), cmdUpper.begin(), ::toupper);

    if (this->_connectMap.count(cmdUpper)) {
        // handleConnection(this->_connectMap[cmdUpper], msg);
        return;
    }

    if (this->_channelMap.count(cmdUpper)) {
        // handleChannel(this->_channelMap[cmdUpper], msg);
        return;
    }

    if (this->_userMap.count(cmdUpper)) {
        // handleUser(this->_userMap[cmdUpper], msg);
        return;
    }

    std::cout << "Unknown command: " << cmdUpper << std::endl;
    
    // std::cout << cmdUpper << std::endl;
}


Command::Command()
{
    initHandlers();
}

Command::~Command()
{
    
}

void Command::initHandlers()
{
    this->_connectMap["CAP"] = CAP;
    this->_connectMap["AUTHENTICATE"] = AUTHENTICATE;
    this->_connectMap["PASS"] = PASS;
    this->_connectMap["NICK"] = NICK;
    this->_connectMap["USER"] = USER;
    this->_connectMap["PING"] = PING;
    this->_connectMap["PONG"] = PONG;
    this->_connectMap["OPER"] = OPER;
    this->_connectMap["QUIT"] = QUIT;
    this->_connectMap["ERROR"] = ERROR;
    this->_channelMap["JOIN"] = JOIN;
    this->_channelMap["PART"] = PART;
    this->_channelMap["TOPIC"] = TOPIC;
    this->_channelMap["NAMES"] = NAMES;
    this->_channelMap["LIST"] = LIST;
    this->_channelMap["INVITE"] = INVITE;
    this->_channelMap["KICK"] = KICK;
    this->_servMap["MOTD"] = MOTD;
    this->_servMap["VERSION"] = VERSION;
    this->_servMap["ADMIN"] = ADMIN;
    this->_servMap["CONNECT"] = CONNECT;
    this->_servMap["LUSERS"] = LUSERS;
    this->_servMap["TIME"] = TIME;
    this->_servMap["STATS"] = STATS;
    this->_servMap["HELP"] = HELP;
    this->_servMap["INFO"] = INFO;
    this->_servMap["MODE"] = MODE;
    this->_userMap["PRIVMSG"] = PRIVMSG;
    this->_userMap["NOTICE"] = NOTICE;
    this->_userMap["WHO"] = WHO;
    this->_userMap["WHOIS"] = WHOIS;
    this->_userMap["WHOWAS"] = WHOWAS;
    this->_operMap["KILL"] = KILL;
    this->_operMap["REHASH"] = REHASH;
    this->_operMap["RESTART"] = RESTART;
    this->_operMap["SQUIT"] = SQUIT;
}

void Command::execute_command(const IRCMessage &msg)
{
    std::string command;
    for (int i = 0; i < msg.command.size(); i++)
        command[i] = std::toupper(msg.command[i]);
    
}

void Command::interpret_command(const IRCMessage &msg)
{
    if (msg.command == "PASS" || )
}

void Command::handleConnection(ConnectMess cmd, const IRCMessage &msg)
{
    
}