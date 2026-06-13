/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 15:00:05 by jjaroens          #+#    #+#             */
/*   Updated: 2026/06/13 15:00:09 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd, int port, const std::string &ip): _fd(fd), _port(port),\
	_ip(ip), _nickname(""), _username(""), _buffer(""), _channels(0), \
	_limitChannel(10), _isAuthenticated(false), _isOperator(false), _issetNick(false),\
	_issetUser(false), _issetPass(false), _iscap(false), _isCapNegotiating(false)
{

}

Client::Client(){}

Client::~Client()
{

}

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
		_nickname = other._nickname;
		_username = other._username;
		_buffer = other._buffer;
		_channels = other._channels;
		_isAuthenticated = other._isAuthenticated;
		_isOperator = other._isOperator;
		_issetNick = other._issetNick;
		_issetUser = other._issetUser;
		_issetPass = other._issetPass;
		_iscap = other._iscap;
		_isCapNegotiating = other._isCapNegotiating;
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

std::string Client::getName() const
{
	return _nickname.empty() ? _ip : _nickname;
}

std::string& Client::getBuffer()
{
	return _buffer;
}

void Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	_username = username;
}

void Client::appendBuffer(const std::string &data)
{
	_buffer += data;
}

void    Client::write(const std::string &message)
{

	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error: sending message");
}


bool Client::setAuthenticated(bool auth)
{
	_isAuthenticated = auth;
	return _isAuthenticated;
}

bool Client::isAuthenticated() const
{
	return _isAuthenticated;
}

bool Client::setOperator(bool op)
{
	_isOperator = op;
	return _isOperator;
}

bool Client::isOperator() const
{
	return _isOperator;
}

bool Client::setNickSet(bool isset)
{
	_issetNick = isset;
	return _issetNick;
}

bool Client::isNickSet() const
{
	return _issetNick;
}

bool Client::setUserSet(bool isset)
{
	_issetUser = isset;
	return _issetUser;
}

bool Client::isUserSet() const
{
	return _issetUser;
}

bool Client::setPassSet(bool isset)
{
	_issetPass = isset;
	return _issetPass;
}

bool Client::isPassSet() const
{
	return _issetPass;
}


void	Client::addChannel(Channel *channel)
{
	_channels.push_back(channel);

}

bool Client::setCap(bool cap)
{
	_iscap = cap;
	return _iscap;
}

bool Client::isCap() const
{
	return _iscap;
}

bool Client::setCapNegotiating(bool negotiating)
{
	_isCapNegotiating = negotiating;
	return _isCapNegotiating;
}

bool Client::isCapNegotiating() const
{
	return _isCapNegotiating;
}

std::vector<Channel*>   Client::getChannels() const
{
	return _channels;
}

int Client::getNumChan() const
{
	return _channels.size();
}

int Client::getLimitChan() const
{
	return _limitChannel;
}

std::string Client::getUsername() const
{
	return _username;
}

void Client::removechannel_from_client(Channel *channel)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (*it == channel)
		{
			_channels.erase(it);
			return;
		}
	}
}