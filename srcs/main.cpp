/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:21:05 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/17 14:42:06 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <csignal>

bool Server::_serverRunning = true;

void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down the server..." << std::endl;
    Server::_serverRunning = false;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    std::signal(SIGINT, signalHandler);
    //check error after
    for (size_t i = 0; argv[1][i]; ++i)
    {
        if (!std::isdigit(argv[1][i]))
        {
            std::cerr << "Error: Port must be a number" << std::endl;
            return 1;
        }
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    if (password.empty())
    {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return 1;
    }

    if (port < 1 || port > 65535)
    {
        std::cerr << "Error: Port must be between 1 and 65535" << std::endl;
        return 1;
    }
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