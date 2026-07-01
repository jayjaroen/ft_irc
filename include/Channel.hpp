/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:29 by jjaroens          #+#    #+#             */
/*   Updated: 2026/06/30 15:43:12 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include "Server.hpp"

# include <ctime> 

class Client;
class Server;

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_setter_topic;
		std::vector<Client *>	_clients; // all client
		std::vector<Client*>	_invited_clients; //new
		std::vector<int>		_operators; // grant +o
		std::time_t				_creationTime;
		std::time_t				_topicSetTime;

		/* Channel modes*/
		std::string				_key; //channel key, linked with k
		size_t					_limit; //limit number of channel members, linked to l
	
		bool					_inviteOnly;
		bool					_topicRestrict;
		bool					_hasKey;
		bool					_hasLimited;
		
		Channel();
		Channel(const Channel &other);
	
	public:
		Channel(const std::string &name, const std::string &key, Client *client);
		~Channel();
		
		std::string				getName() const;
		std::string				getKey() const;
		size_t					getLimit();
		size_t					getChannelSize();
		std::vector<Client *>	getClients();

		void					setKey(std::string key);
		void					setName(std::string name);
		void					setLimit(size_t limit);
		bool					getTopic_mode() const;
		void					setTopic(const std::string &topic, const std::string &setter);
		std::string				getTopic() const;
		void					setInviteOnly(bool inviteOnly);
		bool					isInviteOnly() const;
		bool					isInvited(Client *client) const;
		std::string 			getsetter_topic() const;
		std::string				getCreationTimestr() const;
		std::time_t				getCreationTime() const;
		std::string				getCreationTimeStr_Topic() const;
		std::time_t				getCreationTime_Topic() const;
		size_t					get_operators_size() const;
		
		bool					checkKey(const std::string &key);
		/*Channel actions*/
		void					broadcast(Client *sender, const std::string &message);
		void					response(int fd, const std::string &msg);
		void					broadcastModeChange(Client &sender, const std::string &message);
		void					mode_broadcast(const std::string &message);
		
		void					addClient(Client *client);
		void					removeClient(Client* client);
		void					removeOperator(Client* client);
		void					addInviteClient(Client *client);
		//kick?
		bool					hasClient(Client *client) const;
		bool					isEmpty();
		
		void					addOperator(int fd);
		bool					isOperator(int fd);
		bool					checkOperator(Client &client);

		// Handle modes	
		void					handleInviteMode(Client &sender, const std::string &modeChanges);
		void					handleTopicMode(Client &sender, const std::string &modeChanges);
		void					handleKeyMode(Client &sender, const std::string &modeChanges, const std::string &param);
		void					handleLimitMode(Client &sender, const std::string &modeChanges, const std::string &param);
		void					handleOperatorMode(Client &sender, const std::string &modeChanges, const std::string &nick, Server &server);
};

#endif