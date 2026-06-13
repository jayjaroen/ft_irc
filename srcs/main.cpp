/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:21:05 by jjaroens          #+#    #+#             */
/*   Updated: 2026/06/13 14:31:38 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <csignal>


volatile sig_atomic_t server_signaled = 0;


void signalHandler(int signum) {
	(void)signum;
	server_signaled = 1;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::signal(SIGINT, signalHandler);
	std::signal(SIGQUIT, signalHandler);
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