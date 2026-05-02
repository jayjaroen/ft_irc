/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:29 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/02 15:16:14 by jjaroens         ###   ########.fr       */
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
		std::string				_key; //channel key
		size_t					_limit; //limit number of channel members
		bool					_msgs; // yes/no to external msgs

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
		void					setLimit(size_t limit);
		void					setExtMsg(bool flag);
		
		bool					checkKey(const std::string &key);
		/*Channel actions*/
		void					broadcast(const std::string &message);
		// void					broadcast(const std::string &message, Client* exclude);
		void					addClient(Client *client);
		// void					remove_client(Client* client);
		//kick?
		
		
		
};

#endif
