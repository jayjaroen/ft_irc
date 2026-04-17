/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 11:50:51 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/17 17:46:52 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd, int port, const std::string &ip): _fd(fd), _port(port),\
	_ip(ip)
{
}

Client::Client(){}

Client::~Client(){}

Client::Client(Client const &other)
{
    *this = other;
}

Client& Client::operator=(Client const &other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _port = other._port;
        _ip = other._ip;
    }
    return *this;
}

int Client::getFd() const
{
    return _fd;
}

int Client::getPort() const
{
	return _port;
}

void Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}




