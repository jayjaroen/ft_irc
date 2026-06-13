/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:02:20 by jjaroens          #+#    #+#             */
/*   Updated: 2026/06/13 15:03:24 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "./include/Parser.hpp"

Command::Command() : type(0) {}
Command::~Command(){}

std::string	Command::commandcheck(const std::string input)
{
	size_t	len = 0;
	while (input[len] && !isspace(input[len]))
		len++;
	std::string	word = input.substr(0, len);
	std::string commlist[] = {"CAP", "USER", "QUIT", "JOIN", "PART", "PING", "HELP",
		"NICK", "PASS", "TOPIC", "INVITE", "KICK", "PRIVMSG", "OPER", "MODE", "RESTART"};
	for (size_t i = 0; i < 16; i++)
	{
		if (word == commlist[i])
		{
			this->type = i + 1;
			return word;
		}
	}
	return "";
}

void	Command::msgparser(const std::string input)
{
	std::string	icomm = commandcheck(input);
	std::vector<std::string>	content;
	if (icomm.empty())
	{
		content.push_back(input);
		this->params.push_back(content);
		content.clear();
	}
	else
	{
		if (!(this->params.empty()))
			return;
		if (this->lexer(input, icomm, this->params) == FAILURE)
			return;
	}
}

std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}