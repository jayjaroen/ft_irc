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

void Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

std::string Client::getName() const
{
    return _nickname.empty() ? _ip : _nickname;
}

void Client::appendBuffer(const std::string &data)
{
    _buffer += data;
}

std::string& Client::getBuffer()
{
    return _buffer;
}