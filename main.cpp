/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:30:58 by codespace         #+#    #+#             */
/*   Updated: 2026/04/04 12:47:08 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int main(int ac, char *av[])
{

	(void) ac;
	(void) av;
	char ***mock_parser = new char**[5];

	mock_parser[0] = new char*[1]; *mock_parser[0] = (char *)":Gyeepach!u@h";
	mock_parser[1] = new char*[1]; *mock_parser[1] = (char *)"PRIVMSG";
	mock_parser[2] = new char*[1]; *mock_parser[2] = (char *)"#lobby";
	mock_parser[3] = new char*[1]; *mock_parser[3] = (char *)"Hello world!";
	mock_parser[4] = NULL;

	IRCMessage msg = translateFromParser(mock_parser);

	std::cout << "--- Logic Received Data ---" << std::endl;
	std::cout << "Prefix: " << msg.prefix << std::endl;
	std::cout << "Command: " << msg.command << std::endl;
	std::cout << "Params: ";
	for (size_t i = 0; i < msg.params.size(); ++i)
		std::cout << "[" << msg.params[i] << "] ";
	std::cout << std::endl;

	if (msg.command == "PRIVMSG")
		std::cout << "\nAction: Processing chat message to " << msg.params[0] << std::endl;

	for (int i = 0; i < 4; ++i) delete[] mock_parser[i];
	delete[] mock_parser;

	return 0;
}