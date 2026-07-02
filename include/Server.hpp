#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h> //socket(), bind(), listen(), accept()
# include <poll.h> //poll() and pollfd structure
# include <fcntl.h> //fcnlt() provide control over files to set non-blocking
# include <netinet/in.h>
# include <unistd.h>
# include <cstring>
#include <netinet/tcp.h>

# include <arpa/inet.h> //inet_aton socket programming to convert an IPv4 network address (binary) to ASCII
# include <cstdlib> //atoi
# include <cerrno> //perror
# include <cstdio>
# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"

class Client;
class Channel;

class Server
{
	private:
		int	_server_fd;
		int	_port;
		int	_backlog;
		bool _check_broadcast;
		std::string	_password;
		
		std::vector<pollfd>	_fds;
		std::map<int, Client*>	_clients;
		std::vector<Channel*> _channels;
		
		// std::map<pollfd, std::string[]>	_messagebuffer;
		// std::string _messagebuffer;
		//	_messagebuffer.sendresponse();
		//	_messagevuffer.clear();
		Server();
	
	public:
		static bool _serverRunning;
		Server(int port, std::string password);
		Server(const Server &other);
		~Server();
		Server &operator=(const Server &other);
		
		bool	start();
		int		openSocket();
		bool	setNonBlocking(int fd);
		bool	get_check_broadcast() const;
		bool	set_check_broadcast(bool check);
		// void	setCloexec(int fd);
		bool	bindAndListen(int fd, int port, int backlog);
		void	run();
		std::string get_password() const;
		// std::string append_buffer(std::string buffer);
		// std::string& get_message_send_client_buffer();
		void	acceptNewClient();
		void	disconnectClient(int client_fd);
		void	handleClientMessage(int client_fd);
		std::string get_creation_date() const;

		// Channel*	findUser(const std::string name);
		Channel*	findChannel(const std::string name);
		Channel*	createChannel(const std::string &name, const std::string &key, Client *client);
		Channel*	findOrCreateChannel(const std::string &name, const std::string &key, Client *client);
		void		deleteChannel(const std::string name);
		// void	deleteChannel(const std::string &channel_name);
		Client*		findClient(const std::string name);
		void		enablePollOut(int fd);

		
		// bool	sendMessage(int fd, std::string msg);
};

#endif
