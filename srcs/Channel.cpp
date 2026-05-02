/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:39 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/02 15:51:05 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/Channel.hpp"

Channel::Channel(){}
Channel::~Channel(){}

Channel::Channel(const std::string &name, const std::string &key, Client *admin)
    :_name(name), _admin(admin), _key(key), _limit(0), _msgs(false)
{
    
}

std::string		Channel::getName() const
{
    return _name;
}

Client*		Channel::getAdmin() const
{
    return _admin;
}

std::string		Channel::getKey() const
{
    return _key;
}

size_t	Channel::getLimit()
{
    return _limit;
}

bool	Channel::getExtMsg()
{
    return _msgs;
}

size_t	Channel::getChannelSize()
{
    return _clients.size();
}

void	Channel::setKey(std::string key)
{
    _key = key;
}

void	Channel::setLimit(size_t limit)
{
    _limit = limit;
}

void	Channel::setExtMsg(bool flag)
{
    _msgs = flag;
}

void	Channel::broadcast(const std::string &message)
{
    std::vector<Client*>::iterator it_start = _clients.begin();
    std::vector<Client*>::iterator it_end = _clients.end();
    
    while (it_start != it_end)
    {
        (*it_start)->write(message); //client write message
        it_start++;
    }
}
//write exclude

bool    Channel::checkKey(const std::string &key)
{
    if (_key.empty())
        return true;
    return _key == key; //if the given key same as the set key
}

void	Channel::addClient(Client *client)
{
	_clients.push_back(client);
    std::cout << "In addClient function, client fd: " << client->getFd() << std::endl;
}

// void	Channel::remove_client(Client *client)
// {
	
// }