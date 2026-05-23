/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:39 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/09 16:28:23 by jjaroens         ###   ########.fr       */
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

void	Channel::broadcast(Client *sender, const std::string &message)
{
    for (unsigned long i = 0; i < _clients.size(); i++)
    {
        Client* target = _clients[i];
        if (target == sender)
            continue;
        send(target->getFd(), message.c_str(), message.size(), 0);
    }
    // std::vector<Client*>::iterator it_start = _clients.begin();
    // std::vector<Client*>::iterator it_end = _clients.end();
    
    // while (it_start != it_end)
    // {
    //     (*it_start)->write(message); //client write message
    //     it_start++;
    // }
    
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

bool    Channel::hasClient(Client *client) const
{
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == client)
            return true;
    }
    return false;
}

// void	Channel::remove_client(Client *client)
// {
	
// }

void    Channel::setAdmin(Client *admin)
{
    _admin = admin;
}

void    Channel::setName(std::string name)
{
    _name = name;
}

void    Channel::removeClient(Client *client)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == client)
        {
            _clients.erase(it);
            return;
        }
    }
}

void    Channel::removeOperator(Client *client)
{
    if (_admin == client)
    {
        _admin = NULL;
        if (!_clients.empty())
            _admin = _clients[0]; //reset admin 
    } 
}

bool    Channel::isEmpty()
{
    return _clients.empty();
}