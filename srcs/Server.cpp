/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psenalia <psenalia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 10:59:54 by gyeepach          #+#    #+#             */
/*   Updated: 2026/07/04 14:44:08 by psenalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
// #include <cstdio>

Server::Server(){}

Server::~Server()
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first != -1)
			close(it->first);
		delete it->second;
	}
	_clients.clear();

	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete *it;
	_channels.clear();

	if (_server_fd != -1)
	{
		close(_server_fd);
		_server_fd = -1;
	}
}

Server::Server(int port, std::string password)
{
	_port = port;
	_password = password;
	_backlog = 10;
	_server_fd = -1;
	_check_broadcast = false;
}

Server::Server(Server const &other)
{
	*this = other;
}

Server& Server::operator=(Server const &other)
{
	if (this != &other)
	{
		_port = other._port;
		_password = other._password;
	}
	return *this;
}

bool Server::setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Failed to set non-blocking" << std::endl;
		return false;
	}
	return true;
}

int Server::openSocket()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1)
	{
		std::cerr << "Error opening socket" << std::endl;
		return -1;
	}
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cerr << "Failed to set socket" << std::endl;
		close(_server_fd);
		_server_fd = -1;
		return -1;
	}
	if (!setNonBlocking(_server_fd))
	{
		std::cerr << "Failed to set non-blocking" << std::endl;
		close(_server_fd);
		_server_fd = -1;
		return -1;
	}
	return _server_fd;
}

bool Server::bindAndListen(int fd, int port, int backlog)
{
	//input socket info before binding
	//before binding -> interface & port 
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);//htons converts a long integer (e.g. port, address) to a network representation
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //by all ips
	
	if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { 
		std::perror("Failed to bind");
		//exit(EXIT_FAILURE);
		return false;
	}
	if (listen(fd, backlog) < 0) { 
		std::perror("Failed to listen");
		return false; 
	}
	return true;
}

void Server::disconnectClient(int client_fd)
{
	Client* client = _clients[client_fd];
	std::string quit_msg = std::string("Disconnect client fd: ") + intToString(client_fd) + "\r\n";
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); )
	{
		Channel* chan = *it;
		
		if (chan != NULL && chan->hasClient(client))
		{

			chan->broadcast(*this, client, quit_msg);
			chan->removeClient(client);
			chan->removeOperator(client);
			

			if (chan->isEmpty())
			{
				delete chan;
				it = _channels.erase(it);
				continue;
			}
		}
		it++;
	}
	
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->fd == client_fd)
		{
			_fds.erase(it);
			break;
		}
	}

	::close(client_fd); 


	_clients.erase(client_fd);
	delete client;
}

// void Server::disconnectClient(int client_fd)
// {
//     std::cout << "Disconnect client fd: " << client_fd << std::endl;
//     //close socket
//     close(client_fd);
//     //remove from poll
//     for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
//     {
//         if (it->fd == client_fd)
//         {
//             _fds.erase(it);
//             break;
//         }
//     }
//     //remove from client map
//     std::map<int, Client*>::iterator it = _clients.find(client_fd);
//     if (it != _clients.end())//why checking end
//     {
//         delete it->second;
//         _clients.erase(it);
//     }
// }

void Server::acceptNewClient()
{
	// std::cout << "In accept new client function" << std::endl;
	// while (true)
	// {
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		//extract client info
		
		int client_fd = accept(_server_fd, (struct sockaddr*)&addr, &len);
		
		if (client_fd < 0)
		{
			std::perror("accept failed");
			return;
			// if (errno == EAGAIN || errno == EWOULDBLOCK)
			//     break;
			// else
			// {
				//     std::perror("accept");
				//     break;
				// }
		}
		int port = ntohs(addr.sin_port);
		std::string ip = inet_ntoa(addr.sin_addr);
		// std::cout << "New client: fd: " << client_fd
				// << " ip = " << ip
				// << " port = " << port << std::endl;
		
		if (!setNonBlocking(client_fd))
		{
			std::cerr << "Failed to set non-blocking" << std::endl;
			close(client_fd);
			return;
			// continue;
		}
		pollfd p = {client_fd, POLLIN, 0}; //fd,events,revents
		_fds.push_back(p);
		_clients[client_fd] = new Client(client_fd, port, ip);
	// }
}

void Server::handleClientMessage(int client_fd)
{
	if (_clients.find(client_fd) == _clients.end())
	{
		std::cerr << "Client fd " << client_fd << " not found" << std::endl;
		return;
	}
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	
	int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
	//client disconnect or error
	if (bytes == 0)
	{
		// std::cout << "Client " << client_fd << " disconnected" << std::endl;
		disconnectClient(client_fd);
		return;
	}
	if (bytes < 0) //check condition: 0 -> client close connection, < 0 -> error
	{
		// std::cout << "Client " << client_fd << " disconnected" << std::endl;
		// disconnectClient(client_fd);
		return;
	}
	Client* client = _clients[client_fd];
	//append to existing read buffer (was incorrectly using write buffer)
	client->appendBuffer(std::string(buffer, bytes));
	//get buffer
	std::string &buf = client->getBuffer();
	size_t pos;
	// std::cout << "Am here" << std::endl;
	while ((pos = buf.find("\r\n")) != std::string::npos)
	{
		std::string message = buf.substr(0, pos);//extract msg
		buf.erase(0, pos + 2);
		// std::cout << "Received from client fd " << client_fd << " Client name " << client->getName() << ": [ " << message << " ]" << std::endl;
		/// ****handle command function ****
		Command cmd;
		cmd.msgparser(message);
		cmd.execute_command(*this, *client); 
		if (_clients.find(client_fd) == _clients.end())
		{
			// std::cout << "Client fd " << client_fd << " disconnected during command execution" << std::endl;
			return;
		}
		// if (this->get_check_broadcast() == true)
		// {
		// 	for (size_t i = 0; i < this->_channels.size(); ++i)
		// 	{
		// 		std::string buffer = this->_channels[i]->get_broadcast_buffer();
		// 		if (!(buffer.empty()))
		// 			this->_channels[i]->broadcast(client, buffer);
		// 	}
		// }
		// sending both server and client info
			// --> if (checkBroadcast(p.fd) == true)
			//		broadcast();
			// <--
	}
}

void    Server::deleteChannel(const std::string name)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == name)
		{
			delete *it; 
			_channels.erase(it);
			return;
		}
	}
}

Channel*	Server::findChannel(const std::string name)
{
	for (unsigned long i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return NULL;
}

Channel*	Server::createChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = new Channel(name, key, client);
	_channels.push_back(channel);
	channel->addClient(client);
	return channel;
}

Channel*	Server::findOrCreateChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = findChannel(name);
	if (channel)
	{
		if (!channel->checkKey(key))
		{
			// std::cout << "Wrong key to join channel: " << name << std::endl;
			return NULL;
		}
		if (!channel->hasClient(client))
			channel->addClient(client);//add if not already in the channel
		return channel;
	}
	return createChannel(name, key, client);
}

// worked but slow and no POLLOUT
// void Server::run()
// {
//     pollfd server_fd = {_server_fd, POLLIN, 0};
//     _fds.push_back(server_fd);
//     while (Server::_serverRunning)
//     {
//         int ret = poll(_fds.data(), _fds.size(), 5);//number of fds have event
//         if (ret < 0)
//         {
//             if (errno == EINTR)
//                 continue;
//             throw std::runtime_error("poll failed");
//         }
//         for (int i = 0; i < (int)_fds.size(); i++)
//         {
//             pollfd &p = _fds[i];
//             if (p.revents == 0)
//                 continue;
//             //handle error
//             if (p.revents & (POLLHUP | POLLERR))
//             {
//                 disconnectClient(p.fd);
//                 i--; // Adjust index since we just removed an element
//                 continue;
//             }
//             //new connection
//             if (p.fd == _server_fd)
//             {
//                 acceptNewClient();
//             }
//             else if (p.revents & POLLIN)
//             {
//                 // Check if client still exists before handling
//                 if (_clients.find(p.fd) != _clients.end())
//                 {
//                     handleClientMessage(p.fd);
//                     // Check if client was disconnected during handleClientMessage
//                     if (_clients.find(p.fd) == _clients.end())
//                     {
//                         i--; // Adjust index since we just removed an element
//                     }
//                 }
//             }
//         }
//     }
// }

void Server::run()
{
	pollfd server_fd = {_server_fd, POLLIN, -1};
	_fds.push_back(server_fd);

	while (Server::_serverRunning)
	{
		for (size_t i = 1; i < _fds.size(); i++)
		{
			pollfd &p = _fds[i];
			Client *c = _clients[p.fd];
			
			if (c)
				if (!c->getWriteBuffer().empty())
					p.events |= POLLOUT;
		}

		int ret = poll(_fds.data(), _fds.size(), -1);
		
		if (ret < 0) {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("poll failed");
		}
		for (int i = 0; i < (int)_fds.size(); i++)
		{
			pollfd &p = _fds[i];
			if (p.revents == 0)
				continue;
			if (p.revents & (POLLHUP | POLLERR))
			{
				disconnectClient(p.fd);
				i--; continue;
			}
			if (p.fd == _server_fd)
				acceptNewClient();
			else
			{
				if (p.revents & POLLIN)
				{
					handleClientMessage(p.fd);
					if (_clients.find(p.fd) == _clients.end())
					{
						i--;
						continue;
					}
				}
				if (p.revents & POLLOUT)
				{
					Client *c = _clients[p.fd];
					if (c && !c->getWriteBuffer().empty())
					{
						std::string &buf = c->getWriteBuffer();
						std::cout << "[DEBUG] Sending to fd " << p.fd << " : " << buf;
						int sent = send(p.fd, buf.c_str(), buf.length(), 0);
						if (sent > 0)
						{
							buf.erase(0, sent);
						} 
						else if (sent < 0)
						{
							// if (errno != EAGAIN && errno != EWOULDBLOCK) {
							// 		disconnectClient(p.fd);
							// 		--i;
									continue;
								// }
						}

						if (buf.empty())
							p.events &= ~POLLOUT;
					}
				}
			}
		}
	}
}

// void Server::run()
// {
// 	pollfd server_fd = {_server_fd, POLLIN, 0};
// 	_fds.push_back(server_fd);
// 	while (Server::_serverRunning)
// 	{
// 		int ret = poll(_fds.data(), _fds.size(), 5);//number of fds have event
// 		if (ret < 0)
// 		{
// 			if (errno == EINTR)
// 				continue;
// 			throw std::runtime_error("poll failed");
// 		}
// 		for (int i = 0; i < (int)_fds.size(); ++i)
// 		{
// 			pollfd &p = _fds[i];
// 			if (p.revents == 0)
// 			continue;
// 			//handle error
// 			if (p.revents & (POLLHUP | POLLERR))
// 			{
// 				disconnectClient(p.fd);
// 				i--; // Adjust index since we just removed an element
// 				// printf("ragequit %d\t%d/%d/%d\n", ret, p.fd, p.events, p.revents);
// 				continue;
// 			}
// 			//new connection
// 			if (p.fd == _server_fd)
// 			{
// 				acceptNewClient();
// 				// printf("connect %d\t%d/%d/%d\n", ret, p.fd, p.events, p.revents);
// 			}
// 			else if (p.revents & POLLIN)
// 			{
// 				// Check if client still exists before handling
// 				if (_clients.find(p.fd) != _clients.end())
// 				{
// 					handleClientMessage(p.fd);
// 					if (p.revents & POLLOUT)
// 					{
// 						std::cout << "access POLLOUT 1" << std::endl;
// 					}
// 					// printf("contact %d\t%d/%d/%d\n", ret, p.fd, p.events, p.revents);
// 					// Check if client was disconnected during handleClientMessage
// 					if (_clients.find(p.fd) == _clients.end())
// 						--i; // Adjust index since we just removed an element
// 					// if (p.revents & POLLOUT)
// 					// {
// 					// 	std::cout << "access POLLOUT 2" << std::endl;
// 					// }
// 				}
// 				// if (p.revents & POLLOUT)
// 				// {
// 				// 	std::cout << "access POLLOUT 3" << std::endl;
// 				// }
// 			}
// 			if (p.revents & POLLOUT)
// 			{
				
// 				std::cout << "access POLLOUT" << std::endl;
// 				// printf("boradcast %d\t%d/%d/%d\n", ret, p.fd, p.events, p.revents);
// 				// if (_clients.find(p.fd) != _clients.end())
// 				// {
// 				// 	if (this->get_check_broadcast() == true)
// 				// 	{
// 				// 		for (size_t i = 0; i < this->_channels.size(); ++i)
// 				// 		{
// 				// 			std::string buffer = this->_channels[i]->get_broadcast_buffer();
// 				// 			if (!(buffer.empty()))
// 				// 				this->_channels[i]->test_broadcast(buffer);
// 				// 		}
// 				// 	}
// 				// 	if (_clients.find(p.fd) == _clients.end())
// 				// 		--i;
// 				// }
// 			}
// 		}
// 	}
// }

bool Server::start()
{
	if (!openSocket())
		return false;
	if (!bindAndListen(_server_fd, _port, _backlog))
	{
		close(_server_fd);
		_server_fd = -1;
		return false;
	}
	std::cout << "Server: listening on port: " << _port << std::endl;
	return true;
}


std::string Server::get_password() const
{
	return _password;
}

std::string Server::get_creation_date() const
{
	time_t now = time(0);
	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
	return std::string(buf);
}

Client*	Server::findClient(const std::string name)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getName() == name)
			return it->second;
	}
	return NULL;
}


// std::string& Server::get_message_send_client_buffer()
// {
// 	return _messagebuffer;
// }

// std::string Server::append_buffer(std::string buffer)
// {
// 	this->_messagebuffer = buffer;
// 	this->check_broadcast = true;
// 	return (this->_messagebuffer);
// }

bool Server::get_check_broadcast() const
{
	return this->_check_broadcast;
}

bool Server::set_check_broadcast(bool check)
{
	this->_check_broadcast = check;
	return (this->_check_broadcast);
}

void	Server::enablePollOut(int fd)
{
	for (size_t i = 1; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds[i].events |= POLLOUT;
			break;
		}
	}
}