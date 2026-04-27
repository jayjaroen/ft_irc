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
# include <cstring>
# include <arpa/inet.h> //inet_aton socket programming to convert an IPv4 network address (binary) to ASCII
# include <cstdlib> //atoi
# include <cerrno> //perror
# include "Client.hpp"
# include "../parser.hpp"

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
