/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/04/11 14:30:34 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>

void sendResponse(int fd, const std::string &message)
{
	if (fd <= 0)
		return;
	ssize_t sent = send(fd, message.c_str(), message.length(), MSG_NOSIGNAL);
	if (sent < 0)
	{
		::perror("Failed to send message");
	}
}


void Command::execute_command(Client &sender, Server &server)
{
    ///Debugging: Send a response to the client to confirm that the command is being processed
    // bool pass_check = sender.isPassSet();
    // bool nick_check = sender.isNickSet();
    // bool user_check = sender.isUserSet();
    // sendResponse(sender.getFd(), "Your status: PASS=" + std::to_string(pass_check) + ", NICK=" + std::to_string(nick_check) + ", USER=" + std::to_string(user_check) + "\r\n");
    std::cout << "Sending to FD: " << sender.getFd() << std::endl;
    ////////

    size_t cmdType = this->type;

    std::cout << cmdType << std::endl;
    

    // add after apply with connect pass etc. with server message
    if (sender.isAuthenticated() == false)
    {
        // std::cout << sender.isAuthenticated() << std::endl;
        if (cmdType != PASS && cmdType != NICK && cmdType != USER)
        {
            std::string err = ":ircserver " + intToString(ERR_NOTREGISTERED) + " " + sender.getName() + " :You have not registered\r\n";
            sendResponse(sender.getFd(), err);
            std::cout << "Client fd " << sender.getFd() << " attempted to execute command without registering: " << err << std::endl;
            return ;
        }
    }


    switch (cmdType)
    {
        case CAP:
            std::cout << "Executing CAP..." << std::endl;
            break;
        case USER:
            std::cout <<  "Executing USER..." << std::endl;
            handleUSER(sender);
            break;
        case RESTART:
            std::cout << "Executing RESTART..." << std::endl;
            break;
        case PASS:
            std::cout << "Executing PASS..." << std::endl;
            handlePass(sender, server);
            break;
        case NICK:
            handleNick(sender);
            std::cout << "Executing NICK..." << std::endl;
            break;
        case JOIN:
            // handleJOIN(sender);
            // std::cout << "Joining channel: " << msg.params[0] << std::endl;
            break;
        case PRIVMSG:
            // handlePRIVMSG(sender);
            // std::cout << "Sending message to: " << msg.params[0] << std::endl;
            break;
        case QUIT:
            std::cout << "Client quitting..." << std::endl;
            handleQuit(sender, server);
            break;
        case PONG:
        {
            std::cout << "Received PONG from client fd " << sender.getFd() << std::endl;
            break;
        }
        case PING:
        {
            if (this->params.empty() || this->params[0].empty()) return;
            std::string token = this->params[0][0]; // ดึงสิ่งที่ Client ส่งมา (เช่น ชื่อเซิร์ฟเวอร์หรือเลขสุ่ม)
            std::string pongResponse = ":ircserver PONG ircserver :" + token + "\r\n";
            sendResponse(sender.getFd(), pongResponse);
            break;
        }
        default:
            // std::cout << "Command " << this->params[0][0] << " not implementes" << std::endl;
            break;
    }
    if (sender.isNickSet() == true && sender.isUserSet() == true && sender.isPassSet() == true && sender.isAuthenticated() == false)
    {
        sender.setAuthenticated(true);
        std::cout << "Client FD " << sender.getFd() << " has successfully registered." << std::endl;
        std::string welcomeMsg = ":ircserver " + intToString(RPL_WELCOME) + " " + sender.getName() + " :Welcome to the IRC server!\r\n";
        welcomeMsg += ":ircserver " + intToString(RPL_YOURHOST) + " " + sender.getName() + " :Your host is ircserver, running version 1.0\r\n";
        welcomeMsg += ":ircserver " + intToString(RPL_CREATED) + " " + sender.getName() + " :This server was created on" + server.get_creation_date() + "\r\n";
        sendResponse(sender.getFd(), welcomeMsg);
    }
    if (this->type == 0)
    {
    //     // std::string err = ":ircserver" + std::to_string(UNKNOWN_CMD) + " " + sender.getNick() + " :Unknown command\r\n";
    //     // sendResponse(sender.getFd(), err);
    //     // std::cout << "Unknown command from client fd " << sender.getFd() << ": " << err << std::endl;
        return;
    }
}

void Command::handleNick(Client &sender)
{
    if (this->params.empty() || this->params[0].empty())
        return;
    std::string newNick = this->params[0][0];
    sender.setNick(newNick);
    sender.setNickSet(true);
    std::cout << "Client FD " << sender.getFd() << " changed nick to " << newNick << std::endl;
}



void Command::handlePass(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty())
        return;
    if (this->params[0][0] == server.get_password())
    {
        sender.setPassSet(true);
        std::cout << "Client FD " << sender.getFd() << " authenticated successfully." << std::endl;
    }
    else if (sender.isAuthenticated())
    {
        std::string err = ":ircserver" + intToString(ERR_ALREADYREGISTRED) + " " + sender.getName() + " :You may not register again\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to re-authenticate." << std::endl;
    }
    else
    {
        std::string err = ":ircserver" + intToString(ERR_PASSWDMISMATCH) + " " + sender.getName() + " :Password incorrect\r\n";  
        sendResponse(sender.getFd(), err);  
        sender.setPassSet(false);
    }
}

void Command::handleQuit(Client &sender, Server &server)
{
    sendResponse(sender.getFd(), "Goodbye!\r\n");
    std::cout << "Client FD " << sender.getFd() << " is quitting." << std::endl;
    server.disconnectClient(sender.getFd());

}

void Command::handleUSER(Client &sender)
{
    // if (this->params.empty() || this->params[0].size() < 4)
    //     return;
    std::string username = this->params[0][0];
    std::string hostname = this->params[1][0];
    std::string servername = this->params[2][0];
    std::string realname = this->params[3][0];
    sender.setUsername(username);
    std::cout << "Client FD " << sender.getFd() << " set username to " << username << std::endl;
    sender.setUserSet(true);
}

// void Command::handlePRIVMSG(Client &sender)
// {
//     if (this->params == 0)
//         return;
//     std::string target = msg.params[0]; // first parameter is the target (user or channel)
//     std::string message = msg.params[1]; // second parameter is the message
//     // server.sendMessageToTarget(target, message);
// }

// void Command::handleJOIN(Client &sender)
// {
//     if (this->params == 0)
//         return;
//     std::string channelName = msg.params[0];
//     // server.addClientToChannel(sender.getFd(), channelName);
// }