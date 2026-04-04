/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:27:41 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/04 17:13:51 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <poll.h>
# include <map>
# include <sys/socket.h> //socket(), bind(), listen(), accept()
# include <poll.h> //poll() and pollfd structure
# include <fcntl.h> //fcnlt() provide control over files to set non-blocking
# include <netinet/in.h>
# include <unistd.h>
# include "Client.hpp"

class Server
{
	private:
		int	_server_fd;
		int	_port;
		int	_backlog;
		std::string	_password;
		
		std::vector<pollfd>	_pollfds;
		std::map<int, Client*>	_clients;
		Server();
	
	public:
		Server(int port, std::string password);
		Server(const Server &other);
		~Server();
		Server &operator=(const Server &other);
		
		bool	start();
		int		openSocket();
		bool	setNonBlocking(int fd);
		void	setCloexec(int fd);
		bool	bindAndListen(int fd, int port, int backlog);
		
		bool	handClient(int cliend_fd);
		bool	handleEvents();
		bool	sendMessage(int fd, std::string msg);
};

#endif



