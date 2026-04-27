#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Server.hpp"
# include <iostream>
# include <string>
# include <vector>
# include <map>


class Client
{
    private:
        int _fd;
        int _port;
        std::string _ip;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        
        // bool    _isAuthenticated;
        // bool    _isOperator;
        
        Client();
        
        
        public:
        ~Client();
        Client(int fd, int port, const std::string &ip);
        Client(const Client &src);
        Client &operator=(const Client &other);
        
        int getFd() const;
        int getPort() const;
        std::string getName() const;
        void    setNick(const std::string &nickname);
        void    appendBuffer(const std::string &data);
        std::string& getBuffer();
        // std::string sendResponse(std::string message);
};


#endif