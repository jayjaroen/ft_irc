#include "../include/Client.hpp"

Client::Client(int fd, int port, const std::string &ip): _fd(fd), _port(port),\
	_ip(ip), _nickname(""), _username(""), _buffer("")
{
}

Client::Client(){}

Client::~Client(){}

Client::Client(Client const &other)
{
    *this = other;
}

Client& Client::operator=(Client const &other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _port = other._port;
        _ip = other._ip;
    }
    return *this;
}

int Client::getFd() const
{
    return _fd;
}

int Client::getPort() const
{
	return _port;
}

std::string Client::getName() const
{
	return _nickname.empty() ? _ip : _nickname;
}

std::string& Client::getBuffer()
{
	return _buffer;
}

Channel*	Client::getChannel() const
{
	return _channel;
}

void Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void	Client::setChannel(Channel *channel)
{
	_channel = channel;
}

void Client::appendBuffer(const std::string &data)
{
	_buffer += data;
}

void    Client::write(const std::string &message)
{
    std::string buffer = message + "\r\n";
    if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error: sending message");
}

// void	Client::join(Channel *channel)
// {
// 	_channel = channel;//multiple channel?
// 	channel->addClient(this);

// }