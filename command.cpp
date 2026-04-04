/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/03 16:20:21 by codespace        ###   ########.fr       */
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

