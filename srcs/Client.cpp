#include "../include/Client.hpp"

Client::Client(int fd, int port, const std::string &ip): _fd(fd), _port(port),\
	_ip(ip), _nickname(""), _username(""), _buffer(""), _isAuthenticated(false), _isOperator(false), _issetNick(false),\
    _issetUser(false), _issetPass(false)
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
        _nickname = other._nickname;
        _username = other._username;
        _buffer = other._buffer;
        _isAuthenticated = other._isAuthenticated;
        _isOperator = other._isOperator;
        _issetNick = other._issetNick;
        _issetUser = other._issetUser;
        _issetPass = other._issetPass;
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

void Client::setUsername(const std::string &username)
{
    _username = username;
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

bool Client::setAuthenticated(bool auth)
{
    _isAuthenticated = auth;
    return _isAuthenticated;
}

bool Client::isAuthenticated() const
{
    return _isAuthenticated;
}

bool Client::setOperator(bool op)
{
    _isOperator = op;
    return _isOperator;
}

bool Client::isOperator() const
{
    return _isOperator;
}

bool Client::setNickSet(bool isset)
{
    _issetNick = isset;
    return _issetNick;
}

bool Client::isNickSet() const
{
    return _issetNick;
}

bool Client::setUserSet(bool isset)
{
    _issetUser = isset;
    return _issetUser;
}

bool Client::isUserSet() const
{
    return _issetUser;
}

bool Client::setPassSet(bool isset)
{
    _issetPass = isset;
    return _issetPass;
}

bool Client::isPassSet() const
{
    return _issetPass;
}
