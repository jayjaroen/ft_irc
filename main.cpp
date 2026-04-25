/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:30:58 by codespace         #+#    #+#             */
/*   Updated: 2026/04/11 14:28:21 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "./include/Server.hpp"

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
    irc.run();
    return 0;
}


// int main(int ac, char *av[])
// {

// 	(void) ac;
// 	(void) av;
// 	Command cmd;

// 	std::string str = "PRIVMSG Joe,Fluke,Jay : Hello World";
// 	// cmd.msgparser(str);
// 	// cmd.execute_command();
// 	return 0;
// }