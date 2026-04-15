/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/11 14:30:34 by gyeepach         ###   ########.fr       */
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



void Command::convert_to_upper(IRCMessage &msg)
{
    std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(), ::toupper);
    // std::string cmdUpper = msg.command;
    // std::transform(cmdUpper.begin(), cmdUpper.end(), cmdUpper.begin(), ::toupper);

    // std::cout << "Unknown command: " << cmdUpper << std::endl;
    
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

// void Command::execute_command(const IRCMessage &msg)
// {
//     std::string command;
//     for (int i = 0; i < msg.command.size(); i++)
//         command[i] = std::toupper(msg.command[i]);
    
// }

// void Command::interpret_command(const IRCMessage &msg)
// {
//     if (msg.command == "PASS" || )
// }

// void Command::handleConnection(ConnectMess cmd, const IRCMessage &msg)
// {
    
// }