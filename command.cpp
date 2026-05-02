/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/05/02 15:48:15 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"


void Command::execute_command(Server &server, Client &sender)
{
    // (void)server;
    if (this->type == 0)
    {
        // std::string err = ":ircserver" + std::to_string(UNKNOWN_CMD) + " " + sender.getNick() + " :Unknown command\r\n";
        // sendResponse(sender.getFd(), err);
        // std::cout << "Unknown command from client fd " << sender.getFd() << ": " << err << std::endl;
        return;
    }
    size_t cmdType = this->type;

    std::cout << cmdType << std::endl;
    

    // add after apply with connect pass etc. with server message
    // if (!sender.isRegistered())
    // {
    //     if (cmdType != PASS && cmdType != NICK && cmdType != USER && cmdType != CAP)
    //     {
    //         std::string err = ":ircserver" + std::to_string(ERR_NOTREGISTERED) + " " + sender.getNick() + " :You have not registered\r\n";
    //         // sendResponse(sender.getFd(), err);
    //         std::cout << "Client fd " << sender.getFd() << " attempted to execute command without registering: " << err << std::endl;
    //         return ;
    //     }
    // }
// 
    switch (cmdType)
    {
        case CAP:
            std::cout << "Executing CAP..." << std::endl;
            break;
        case CONNECT:
            std::cout <<  "Executing CONNECT..." << std::endl;
            break;
        case RESTART:
            std::cout << "Executing RESTART..." << std::endl;
            break;
        case PASS:
            std::cout << "Executing PASS..." << std::endl;
            // handlePass(sender);
            break;
        case NICK:
            handleNick(sender);
            std::cout << "Executing NICK..." << std::endl;
            break;
        case JOIN:
            handleJOIN(server, sender);
            // std::cout << "Joining channel: " << msg.params[0] << std::endl;
            break;
        case PRIVMSG:
            // handlePRIVMSG(sender);
            // std::cout << "Sending message to: " << msg.params[0] << std::endl;
            break;
        case QUIT:
            std::cout << "Client quitting..." << std::endl;
            // handleQuit(sender);
            break;
        case PING:
            // std::cout << "Received PING, sending PONG..." << std::endl;
            // std::string pongResponse = "PONG :" + sender.getNick() + "\r\n";
            // sendResponse(sender.getFd(), pongResponse);
            break;
        default:
            // std::cout << "Command " << this->params[0][0] << " not implementes" << std::endl;
            break;
    }
}

void Command::handleNick(Client &sender)
{
    if (this->params.empty() || this->params[0].empty())
        return;
    std::string newNick = this->params[0][0];
    sender.setNick(newNick);
    std::cout << "Client FD " << sender.getFd() << " changed nick to " << newNick << std::endl;
}



// void Command::handlePass(Client &sender)
// {
//     if (this->params == 0)
//         return;
//     // if (msg.params[0] == serverPassword)
//     //     sender.setAuthenticated(true);
//     //     std::cout << "Client FD " << sender.getFd() << " authenticated successfully." << std::endl;
//     // else if (sender.isAuthenticated())
//     // {
//     //     std::string err = ":ircserver" + std::to_string(ALREADY_REG) + " " + sender.getNick() + " :You may not register again\r\n";
//     //     sendResponse(sender.getFd(), err);
//     //     std::cout << "Client FD " << sender.getFd() << " attempted to re-authenticate." << std::endl;
//     // }
//     // else
//     // {
//     //     std::string err = ":ircserver" + std::to_string(PASS_ERR) + " " + sender.getNick() + " :Password incorrect\r\n";  
//     //     sendResponse(sender.getFd(), err);  
//     //     sender.setAuthenticated(false);
//     // }
// }

// void Command::handleQuit(Client &sender)
// {
//     // sendResponse(sender.getFd(), "Goodbye!\r\n");
//     std::cout << "Client FD " << sender.getFd() << " is quitting." << std::endl;
//     // server.disconnectClient(sender.getFd());
// }

// void Command::handlePRIVMSG(Client &sender)
// {
//     if (this->params == 0)
//         return;
//     std::string target = msg.params[0]; // first parameter is the target (user or channel)
//     std::string message = msg.params[1]; // second parameter is the message
//     // server.sendMessageToTarget(target, message);
// }

void Command::handleJOIN(Server &server, Client &sender)
{
    std::cout << "I'm in handle join function" << std::endl;
    if (this->params.empty())
        return;
    std::cout << "When having params " << std::endl;
    std::string key = "";
    std::cout << "am here2" << std::endl;
    if (params[1].empty())
        key = this->params[1][0];
    std::cout << "am here1" << std::endl;
    std::string channel_name = this->params[0][0];
    std::cout << "Channel name is: " << channel_name << " key is " << key << std::endl;
    server.findOrCreateChannel(channel_name, key, &sender);
    // server.addClientToChannel(sender.getFd(), channelName);
}