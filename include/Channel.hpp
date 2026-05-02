/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:29 by jjaroens          #+#    #+#             */
/*   Updated: 2026/05/02 10:23:49 by jjaroens         ###   ########.fr       */
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
		std::string				_k; //channel key
		size_t					_l; //limit number of channel members
		bool					_n; // yes/no to external msgs

		Channel();
		Channel(const Channel &other);
	
	public:
		Channel(const std::string &name, const std::string &key, Client *admin);
		~Channel();
		
		std::string				get_name() const;
		Client*					get_admin() const;
		std::string				get_key() const;
		size_t					get_limit();
		bool					get_ext_msg();
		size_t					get_channel_size();

		void					set_key(std::string key);
		void					set_limit(size_t limit);
		void					set_ext_msg(bool flag);
		
		/*Channel actions*/
		void					broadcast(const std::string &message);
		// void					broadcast(const std::string &message, Client* exclude);
		void					add_client(Client* client);
		// void					remove_client(Client* client);
		//kick?
		
		
		
};

#endif
