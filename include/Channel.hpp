/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psenalia <psenalia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:29 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/09 16:23:49 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"

class Client;

class Channel
{
    private:
		std::string				_name;
		Client*					_admin;
		std::vector<Client *>	_clients;

		/* Channel modes*/
		std::string				_key; //channel key, linked with k
		size_t					_limit; //limit number of channel members, linked to l
		bool					_msgs; // yes/no to external msgs
		// enum					_mode; //indicate the mode (such as k, l, t, o, i)

		Channel();
		Channel(const Channel &other);
	
	public:
		Channel(const std::string &name, const std::string &key, Client *admin);
		~Channel();
		
		std::string				getName() const;
		Client*					getAdmin() const;
		std::string				getKey() const;
		size_t					getLimit();
		bool					getExtMsg();
		size_t					getChannelSize();

		void					setKey(std::string key);
		void					setAdmin(Client *admin);
		void					setName(std::string name);
		void					setLimit(size_t limit);
		void					setExtMsg(bool flag);
		
		bool					checkKey(const std::string &key);
		/*Channel actions*/
		void					broadcast(Client *sender, const std::string &message);
		// void					broadcast(const std::string &message, Client* exclude);
		void					addClient(Client *client);
		void					removeClient(Client* client);
		void					removeOperator(Client* client);
		//kick?
		bool					hasClient(Client *client) const;
		bool					isEmpty();
};

#endif
