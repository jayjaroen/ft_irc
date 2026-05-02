#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Server.hpp"
# include "Channel.hpp"
# include <iostream>
# include <string>
# include <vector>
# include <map>

class Channel;

class Client
{
    private:
        int _fd;
        int _port;
        std::string _ip;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        Channel*    _channel;
        //client join many channels?
        // std::vector<Channel*>   _channels;
        
        bool    _isAuthenticated;
        bool    _isOperator;
        bool    _issetNick;
        bool    _issetUser;
        bool    _issetPass;
        
        Client();
        
        
        public:
        ~Client();
        Client(int fd, int port, const std::string &ip);
        Client(const Client &src);
        Client &operator=(const Client &other);
        
        //getter
        int getFd() const;
        int getPort() const;
        std::string getName() const;
        void    setUsername(const std::string &username);
        bool    setAuthenticated(bool auth);
        bool    setNickSet(bool isset);
        bool    isNickSet() const;
        bool    setUserSet(bool isset);
        bool    isUserSet() const;
        bool    setPassSet(bool isset);
        bool    isPassSet() const;
        bool    isAuthenticated() const;
        bool    setOperator(bool op);
        bool    isOperator() const;
        std::string& getBuffer();
        Channel*    getChannel() const;

        //setter
        void    setNick(const std::string &nickname);
        void    setChannel(Channel *channel);
        
        // Responses
        // std::string sendResponse(std::string message);
        
        void    appendBuffer(const std::string &data);
        void    write(const std::string &message);
        // void    join(Channel *channel);

};


#endif