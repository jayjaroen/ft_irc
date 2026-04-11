/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:21:05 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/11 12:53:44 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    //check error after
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    Server irc(port, password);
    try
    {
        irc.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error:" << e.what() << std::endl;
    }
    
    return 0;
}