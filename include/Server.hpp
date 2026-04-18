/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:27:41 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/18 10:29:37 by jjaroens         ###   ########.fr       */
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
# include <cstdlib>
# include <cstring>
# include <cerrno>
# include <cstdio>
# include <arpa/inet.h> //inet_aton socket programming to convert an IPv4 network address (binary) to ASCII
# include "Client.hpp"

class Client;

class Server
{
	private:
		int	_server_fd;
		int	_port;
		int	_backlog;
		std::string	_password;
		
		std::vector<pollfd>	_fds;
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
		void	run();
		void	acceptNewClient();
		void	disconnectClient(int client_fd);
		void	handleClientMessage(int client_fd);
		
		// bool	sendMessage(int fd, std::string msg);
};

#endif



