/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/18 15:57:50 by jjaroens         ###   ########.fr       */
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


// เอาออกไป
void Command::convert_to_upper(IRCMessage &msg)
{
    std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(), ::toupper);
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
    this->_commandprompts["CAP"] = CAP;
    this->_commandprompts["CONNECT"] = CONNECT;
    this->_commandprompts["QUIT"] = QUIT;
    this->_commandprompts["JOIN"] = JOIN;
    this->_commandprompts["PART"] = PART;
    this->_commandprompts["PING"] = PING;
    this->_commandprompts["HELP"] = HELP;
    this->_commandprompts["NICK"] = NICK;
    this->_commandprompts["PASS"] = PASS;
    this->_commandprompts["TOPIC"] = TOPIC;
    this->_commandprompts["INVITE"] = INVITE;
    this->_commandprompts["KICK"] = KICK;
    this->_commandprompts["PRIVMSG"] = PRIVMSG;
    this->_commandprompts["OPER"] = OPER;
    this->_commandprompts["MODE"] = MODE;
    this->_commandprompts["RESTART"] = RESTART;
}

void Command::execute_command(IRCMessage &msg, Client &sender)
{
    this->convert_to_upper(msg);
    if (this->_commandprompts.count(msg.command) == 0)
    {
        std::cout << "Error: Unknown command" << msg.command << std::endl;
        return ;
    }
    CommandPrompts cmdType = this->_commandprompts[msg.command];
    
    // if (!sender.isRegistered())
    // {
    //     if (cmdType != PASS && cmdType != NICK && cmdType != USER && cmdType != CAP)
    //     {
    //         // send ERR_NOTREGISTERED
    //         return ;
    //     }
    // }

    switch (cmdType)
    {
        case CAP:
            std::cout << "Executing CAP..." << std::endl;
            break;
        case CONNECT:
            std::cout <<  "Executing CONNECT..." << std::endl;
            break;
        case RESTART:
            std::cout << "Executing RESTART..." << std::endl;
            break;
        case PASS:
            std::cout << "Executing PASS..." << std::endl;
            break;
        case NICK:
            handleNick(msg, sender);
            std::cout << "Executing NICK..." << std::endl;
            break;
        case JOIN:
            std::cout << "Joining channel: " << msg.params[0] << std::endl;
            break;
        case PRIVMSG:
            std::cout << "Sending message to: " << msg.params[0] << std::endl;
            break;
        case QUIT:
            std::cout << "Client quitting..." << std::endl;
            break;
        default:
            std::cout << "Command " << msg.command << " not implementes" << std::endl;
            break;
    }
}

void Command::handleNick(IRCMessage &msg, Client &sender)
{
    if (msg.params.empty())
    {
        return;
    }
    sender.setNick(msg.params[0]);
    std::cout << "Client FD " << sender.getFd() << " changed nick to " << msg.params[0] << std::endl;
}

