/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/05 22:17:32 by gyeepach         ###   ########.fr       */
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

// void Command::execute_command(const IRCMessage &msg)
// {
//     if (msg.command)
// }

void Command::interpret_command(const IRCMessage &msg)
{
    if (msg.command == "PASS" || )
}

void Command::handleConnection(ConnectMess cmd, const IRCMessage &msg)
{
    
}