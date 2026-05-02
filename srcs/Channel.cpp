/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:39 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/02 10:23:42 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/Channel.hpp"

Channel::Channel(){}
Channel::~Channel(){}

Channel::Channel(const std::string &name, const std::string &key, Client* admin)
    :_name(name), _admin(admin), _k(key), _l(0), _n(false)
{
    
}

std::string		Channel::get_name() const
{
    return _name;
}

Client*		Channel::get_admin() const
{
    return _admin;
}

std::string		Channel::get_key() const
{
    return _k;
}

size_t	Channel::get_limit()
{
    return _l;
}

bool	Channel::get_ext_msg()
{
    return _n;
}

size_t	Channel::get_channel_size()
{
    return _clients.size();
}

void	Channel::set_key(std::string key)
{
    _k = key;
}

void	Channel::set_limit(size_t limit)
{
    _l = limit;
}

void	Channel::set_ext_msg(bool flag)
{
    _n = flag;
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

void	Channel::add_client(Client *client)
{
	_clients.push_back(client);
}

// void	Channel::remove_client(Client *client)
// {
	
// }