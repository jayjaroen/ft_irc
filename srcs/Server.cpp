/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:06 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/04 17:30:37 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

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
        perror("Failed to bind");
        //exit(EXIT_FAILURE);
        return false;
    }
    if (listen(fd, backlog) < 0) { 
        perror("Failed to listen");
        return false; 
    }
    return true;
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
