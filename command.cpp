/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psenalia <psenalia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/05/22 15:41:54 by psenalia         ###   ########.fr       */
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


void Command::execute_command(Server &server, Client &sender)
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
            handleJOIN(server, sender);
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
        case MODE:
            std::cout << "Executing MODE..." << std::endl;
            handleMODE(sender, server);
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

void Command::handleJOIN(Server &server, Client &sender)
{
    if (this->params.empty())
        return;
    std::string key = "";
    std::string channel_name = this->params[0][0];
    if (this->params.size() > 1 && !this->params[1].empty())
        key = this->params[1][0];
    std::cout << "Channel name is: " << "\"" << channel_name << "\"" << " key is " << "\"" << key << "\"" << std::endl;
    server.findOrCreateChannel(channel_name, key, &sender);
}

void Command::handleMODE(Client &sender, Server &server)
{
    std::string modeTarget = this->params[0][0]; // The target of the MODE command (channel or user)
    std::string modeChanges = this->params[1][0]; // The mode changes
    bool client_or_channel = (modeTarget[0] == '#'); // Determine if the target is a channel or a user based on the first character
    if (client_or_channel)
    {
        if (modeChanges.empty())
        {
            std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
            sendResponse(sender.getFd(), err);
            std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
            return;
        }
        if (modeChanges[0] != '+' && modeChanges[0] != '-')
        {
            std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[0] + " :Unknown MODE flag\r\n";
            sendResponse(sender.getFd(), err);
            std::cout << "Client FD " << sender.getFd() << " attempted to change modes with invalid mode string: " << modeChanges << std::endl;
            return;
        }
        // Handle channel mode changes
        Channel* channel = server.findChannel(modeTarget);
        if (channel)
        {
            switch (modeChanges[1]) // Example: Check the second character for specific mode flags
            {
                case 'k': // Channel key
                    std::cout << "Handling channel key mode change for channel: " << modeTarget << std::endl;
                    // Implement logic to set or remove channel key
                    break;
                case 'l': // User limit
                    std::cout << "Handling user limit mode change for channel: " << modeTarget << std::endl;
                    // Implement logic to set or remove user limit
                    break;
                case 't': // restrictions of the TOPIC channel
                    std::cout << "Handling t channel mode change for channel: " << modeTarget << std::endl;
                    // Implement logic to set or remove moderated channel mode
                    break;
				case 'o': // operator given mode
					std::cout << "Handling operator given mode change for channel: " << modeTarget << std::endl;
					break;
				case 'i': // invite-only mode
					std:: cout << "Handling Invite only channel mode change for channel: " << modeTarget << std::endl;
					break;
                default:
                    std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
                    sendResponse(sender.getFd(), err);
                    std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
                    return;
            }
            // Here you would implement the logic to change the channel modes based on modeChanges
        }
        else
        {
            std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + modeTarget + " :No such channel\r\n";
            sendResponse(sender.getFd(), err);
            std::cout << "Client FD " << sender.getFd() << " attempted to change modes for non-existent channel: " << modeTarget << std::endl;
        }
    }
    else
    {
		return ;
        // // Handle user mode changes
        // Client* targetClient = server.findUser(modeTarget);
        // if (targetClient)
        // {
        //     switch (modeChanges[1])
        //     {
        //         case 'o':
        //             std::cout << "Handling operator status mode change for user: " << modeTarget << std::endl;
        //             break;
        //         case 'i': // Invisible mode
        //             std::cout << "Handling invisible mode change for user: " << modeTarget << std::endl;
        //             break;
        //         default:
        //             std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :Unknown MODE flag\r\n";
        //             sendResponse(sender.getFd(), err);
        //             std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
        //             return;
        //     }
        // }
        // else
        // {
        //     std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + modeTarget + " :No such nick\r\n";
        //     sendResponse(sender.getFd(), err);
        //     std::cout << "Client FD " << sender.getFd() << " attempted to change modes for non-existent user: " << modeTarget << std::endl;
        // }
    }
    // std::cout << "Handling MODE command for client fd " << sender.getFd() << std::endl;
}

// void Command::handleOPER(Client &sender, Server &server)
// {
//     if (this->params.empty())
//         return;
//     std::string target = this->params[0][0];
// 	std::string passwd = this->params[1][0];
// 	if (server.getOperUser == target)
// 	{
// 		std::string err = ":ircserver " + intToString(ERR_NOOPERHOST) + " " + sender.getName() + " " + modeTarget + " :No 0-lines for your host\r\n";
//         sendResponse(sender.getFd(), err);
//         std::cout << "Client FD " << sender.getFd() << " requested operator priviledge with incorrect operator username: " << modeTarget << std::endl;
// 		return;
//     }
// 	if (server.getOperPass == passwd)
// 	{
// 		std::string err = ":ircserver " + intToString(ERR_PASSWDMISMATCH) + " " + sender.getName() + " " + modeTarget + " :Password incorrect\r\n";
//         sendResponse(sender.getFd(), err);
//         std::cout << "Client FD " << sender.getFd() << " requested operator priviledge with incorrect operator password: " << modeTarget << std::endl;
// 		return;
//     }
//     sender.setOperator(sender);
// 	std::string reply = ":ircserver " + intToString(RPL_YOUREOPER) + " " + sender.getName() + " " + modeTarget + " :You are now an IRC operator\r\n";
// 	sendResponse(sender.getFd(), reply);
//     std::cout << "Client FD " << sender.getFd() << sender.getName() << " gained IRC operator priviledges." << std::endl;
// }