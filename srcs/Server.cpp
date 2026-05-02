
#include "../include/Server.hpp"
// #include <cstdio>

Server::Server(){}

Server::~Server(){}

Server::Server(int port, std::string password)
{
    _port = port;
    _password = password;
    _backlog = 10; //number of connections can wait
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
    int flag = fcntl(fd, F_GETFL); //receive status
    if (flag == -1)
        return false;
    return (fcntl(fd, F_SETFL, flag | O_NONBLOCK) != -1);
}

void Server::setCloexec(int fd)
{
    int fd_flag = fcntl(fd, F_GETFD);
    if (fd_flag != -1)
        fcntl(fd, F_SETFD, fd_flag | FD_CLOEXEC);
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
    setCloexec(_server_fd);
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
    std::cout << "Disconnect client fd: " << client_fd << std::endl;
    //close socket
    close(client_fd);
    //remove from poll
    for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
    {
        if (it->fd == client_fd)
        {
            _fds.erase(it);
            break;
        }
    }
    //remove from client map
    std::map<int, Client*>::iterator it = _clients.find(client_fd);
    if (it != _clients.end())//why checking end
    {
        delete it->second;
        _clients.erase(it);
    }
}

void Server::acceptNewClient()
{
    std::cout << "In accept new client function" << std::endl;
    while (true)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        
        int client_fd = accept(_server_fd, (struct sockaddr*)&addr, &len);
        
        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
            {
                std::perror("accept");
                break;
            }
        }
        //extract client info
        int port = ntohs(addr.sin_port);
        std::string ip = inet_ntoa(addr.sin_addr);
        std::cout << "New client: fd: " << client_fd
                << " ip = " << ip
                << " port = " << port << std::endl;
        
        if (!setNonBlocking(client_fd))
        {
            std::cerr << "Failed to set non-blocking" << std::endl;
            close(client_fd);
            continue;
        }
        pollfd p = {client_fd, POLLIN, 0}; //fd,events,revents
        _fds.push_back(p);
        _clients[client_fd] = new Client(client_fd, port, ip);
    }
}

void Server::handleClientMessage(int client_fd)
{
    std::cout << "----In handle client function----" << std::endl;
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    
    int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    //client disconnect or error
    if (bytes <= 0) //check condition: 0 -> client close connection, < 0 -> error
    {
        std::cout << "Client " << client_fd << " disconnected" << std::endl;
        disconnectClient(client_fd);
        return;
    }
    Client* client = _clients[client_fd];
    //append to existing buffer
    client->appendBuffer(std::string(buffer, bytes));
    //get buffer
    std::string &buf = client->getBuffer();
    size_t pos;
    std::cout << "Am here" << std::endl;
    while ((pos = buf.find("\r\n")) != std::string::npos)
    {
        std::string message = buf.substr(0, pos);//extract msg
        buf.erase(0, pos + 2); //remove proceed msg from buffer
        std::cout << "Received from client fd " << client_fd << " Client name " << client->getName() << ": [ " << message << " ]" << std::endl;
        /// ****handle command function ****
        Command cmd;
        cmd.msgparser(message);
        // cmd.execute_command(*client);
        cmd.execute_command(*this, *client); 
		// sending both server and client info
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
	return channel;
}

Channel*	Server::findOrCreateChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = findChannel(name);
	if (channel)
	{
		if (!channel->checkKey(key))
		{
			std::cout << "Wrong key to join channel: " << name << std::endl;
			return NULL;
		}
		channel->addClient(client);
		return channel;
	}
	return createChannel(name, key, client);
}



void Server::run()
{
    pollfd server_fd = {_server_fd, POLLIN, 0};
    _fds.push_back(server_fd);
    while (true)
    {
        int ret = poll(_fds.data(), _fds.size(), 1000);//number of fds have event
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("poll failed");
        }
        for (int i = 0 ; i < (int)_fds.size(); i++)
        {
            pollfd &p = _fds[i];
            if (p.revents == 0)
                continue;
            //handle error
            if (p.revents & (POLLHUP | POLLERR))
            {
                disconnectClient(p.fd);
                i--; // Adjust index since we just removed an element
                continue;
            }
            //new connection
            if (p.fd == _server_fd)
            {
                acceptNewClient();
            }
            else if (p.revents & POLLIN)
            {
                // Check if client still exists before handling
                if (_clients.find(p.fd) != _clients.end())
                {
                    handleClientMessage(p.fd);
                    // Check if client was disconnected during handleClientMessage
                    if (_clients.find(p.fd) == _clients.end())
                    {
                        i--; // Adjust index since we just removed an element
                    }
                }
            }
        }
    }
}

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
