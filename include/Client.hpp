/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:49:24 by jjaroens          #+#    #+#             */
/*   Updated: 2026/04/04 14:57:02 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Server.hpp"


class Client
{
    private:
        int _fd;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        bool    _isAuthenticated;
        bool    _isOperator;
    
    public:
        int getFd();
        void    setNick(std::string nickname);
        std::string sendResponse(std::string message);
};


#endif