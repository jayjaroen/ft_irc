/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:49:24 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/17 17:46:25 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Server.hpp"


class Client
{
    private:
        int _fd;
        int _port;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        std::string _ip;
        
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
        
        void    setNick(const std::string &nickname);
        // std::string sendResponse(std::string message);
};


#endif