/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/05/23 16:27:39 by jjaroens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <string>

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
        if (cmdType != PASS && cmdType != NICK && cmdType != USER && cmdType != CAP)
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
            sendResponse(sender.getFd(), "CAP * LS :\r\n");
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
            break;
        case PRIVMSG:
            handlePRIVMSG(server,sender);
            break;
        case QUIT:
            std::cout << "Client quitting..." << std::endl;
            handleQuit(sender, server);
            break;
        case MODE:
            std::cout << "Executing MODE..." << std::endl;
            handleMODE(sender, server);
            break;
        case HELP:
        {
            handleHELP(sender, server);
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
        case PART:
        {
            handlePart(server, sender);
            break;
        }
        case INVITE:
        {
            handleINVITE(sender, server);
            break;
        }
        case KICK:
        {
            handleKICK(sender, server);
            break;
        }
        // case PRIVMSG:
        // {
        //     // if (this->params.empty() || this->params[0].empty() || this->params[1].empty())
        //     //     return;
        //     // std::string target = this->params[0][0]; // first parameter is the target (user or channel)
        //     // std::string message = this->params[1][0]; // second parameter is the message
        //     // server.sendMessageToTarget(target, message);
        //     break;
        // }
        case TOPIC:
        {
            // handleTOPIC(sender, server);
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
    if (this->params.empty() || this->params.size() != 4)
        return;
    std::string username = this->params[0][0];
    std::string hostname = this->params[1][0];
    std::string servername = this->params[2][0];
    std::string realname = this->params[3][0];
    sender.setUsername(username);
    std::cout << "Client FD " << sender.getFd() << " set username to " << username << std::endl;
    sender.setUserSet(true);
}

void Command::handlePRIVMSG(Server &server, Client &sender)
{
    std::cout << "I'm in private message function" << std::endl;
    if (this->params.empty())
        return;
    std::string target = this->params[0][0]; // first parameter is the target (user or channel)
    std::string message = this->params[1][0]; // second parameter is the message
    // in case channel
    std::cout << "The channel name is " << target << std::endl;
    if (target[0] == '#' )
    {
        Channel* channel = server.findChannel(target);
        if (!channel)
        {
            std::cout << "Channel not found" << std::endl;
            return;
        }
        if (!channel->hasClient(&sender))
        {
            std::cout << "Sender does not belong to " << target << std::endl;
            return;
        }
        std::string text = ":" + sender.getName() + " PRIVMSG " + target + " : " + message + "\r\n";
        channel->broadcast(&sender, text);
    }
    else
    {
        Client *target_client = server.findClient(target);
        if (!target_client)
            return;
        std::string text = ":" + sender.getName() + " PRIVMSG " + target + " : " + message + "\r\n";
        send(target_client->getFd(), text.c_str(), text.size(), 0);
    }
    // server.sendMessageToTarget(target, message);
}

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

void    Command::handlePart(Server &server, Client &sender)
{
    if (this->params.empty())
        return;
    std::string channel_name = this->params[0][0];
    Channel* channel = server.findChannel(channel_name);
    if (!channel)
    {
        std::cout << "The channel is not found" << std::endl;
        return;
    }
    if (!channel->hasClient(&sender))
    {
        std::cout << "Client does not belong to the channel " << channel_name << std::endl;
        return;
    }
    std::string message = ":" + sender.getName() + " PART " + channel_name + "\r\n";
    channel->broadcast(&sender, message);
    channel->removeClient(&sender);
    channel->removeOperator(&sender);
    std::cout << sender.getName() << " left channel " << channel_name << std::endl;
    if (channel->isEmpty())
    {
        std::cout << "Deleting the channel " << channel_name << std::endl;
        server.deleteChannel(channel_name);
    }
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
                    channel->handleKeyMode(sender, modeChanges, this->params[2][0]); //third argument
                    // Implement logic to set or remove channel key
                    break;
                case 'l': // User limit
                    std::cout << "Handling user limit mode change for channel: " << modeTarget << std::endl;
                    channel->handleLimitMode(sender, modeChanges, this->params[2][0]);
                    // Implement logic to set or remove user limit
                    break;
                case 't': // restrictions of the TOPIC channel
                    std::cout << "Handling t channel mode change for channel: " << modeTarget << std::endl;
                    channel->handleTopicMode(sender, modeChanges);
                    // Implement logic to set or remove moderated channel mode
                    break;
				case 'o': // operator given mode
					std::cout << "Handling operator given mode change for channel: " << modeTarget << std::endl;
                    // if (modeChanges[0] == '+')
                    //     // channel.admin.push_back(sender); // Need function to add co-admin, and change _admin type to vector<Clients*>
                    // else if (modeChanges[0] == '-')
                    //     // channel.admin.remove(sender); // Need function to remove admin
                    // else
                    // {
                    //     std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
                    //     sendResponse(sender.getFd(), err);
                    //     std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
                    // }
					break;
				case 'i': // invite-only mode
					std:: cout << "Handling Invite only channel mode change for channel: " << modeTarget << std::endl;
                    channel->handleInviteMode(sender, modeChanges);
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

// These are function not testing yet, so I will implement them later after I finish the main functions of the server and client

void Command::handleHELP(Client &sender, Server &server)
{
    (void) server;
    std::string cmd = this->params[0][0];

    
    if (this->params.empty() || this->params[0].empty())
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + "* :** Help System **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + "* :Available commands: JOIN, PART, PING, NICK, TOPIC, INVITE, KICK, PRIVMSG, MODE, CAP, USER, PASS\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + "* :/HELP USERCMDS to list available\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + "* :commands, or join the #help channel\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "JOIN")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " JOIN :** The JOIN command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " JOIN :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " JOIN :The /JOIN command is used to join a specific channel on the IRC server.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " JOIN :Usage: /JOIN <channel>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "PART")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " PART :** The PART command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PART :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PART :The /PART command is used by a client to leave a specific channel on the IRC server. The client must provide the name of the channel they wish to leave. Once the client successfully parts from the channel, they will no longer receive messages from that channel and will be removed from the channel's member list.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " PART :Usage: /PART <channel>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "PING")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " PING :** The PING command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PING :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PING :The /PING command is used by a client to check the connectivity and responsiveness of the IRC server. When a client sends a PING command, the server responds with a PONG message, indicating that the server is alive and reachable. This command is often used to keep the connection between the client and server active and to detect any potential network issues.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " PING :Usage: /PING <server>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "NICK")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " NICK :** The NICK command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " NICK :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " NICK :The /NICK command is used by a client to set or change their nickname on the IRC server. The client must provide the desired nickname as a parameter. If the nickname is available and not already in use by another client, the server will update the client's nickname accordingly. If the nickname is already taken, the server will respond with an error message, prompting the client to choose a different nickname.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " NICK :Usage: /NICK <name>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "TOPIC")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " TOPIC :** The TOPIC command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " TOPIC :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " TOPIC :The /TOPIC command is used by a client to set or view the topic of a specific channel on the IRC server. To set the topic, the client must provide the channel name and the desired topic as parameters. If the client has the necessary permissions, the server will update the channel's topic accordingly. To view the current topic of a channel, the client can simply provide the channel name as a parameter, and the server will respond with the current topic of that channel.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " TOPIC :Usage: /TOPIC <channel> [topic]\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "INVITE")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " INVITE :** The INVITE command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " INVITE :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " INVITE :The /INVITE command is used by a client to invite another user to join a specific channel on the IRC server. The client must provide the nickname of the user they wish to invite and the name of the channel they want to invite them to. If the invited user accepts the invitation, they will be able to join the specified channel and participate in its discussions.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " INVITE :Usage: /INVITE <user> <channel>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "KICK")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " KICK :** The KICK command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " KICK :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " KICK :The /KICK command is used by a client to remove another user from a specific channel on the IRC server. The client must provide the nickname of the user they wish to kick and the name of the channel they want to kick them from. If the client has the necessary permissions, the server will remove the user from the channel and notify them of the action.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " KICK :Usage: /KICK <user> <channel>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "PRIVMSG")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " PRIVMSG :** The PRIVMSG command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PRIVMSG :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PRIVMSG :The /PRIVMSG command is the main way\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " PRIVMSG :to send messages to other users.\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "MODE")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " MODE :** The MODE command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " MODE :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " MODE :The /MODE command is used by a client to change or view the modes of a specific channel or user on the IRC server. To change modes, the client must provide the target (either a channel name or a user's nickname) and the mode changes as parameters. The mode changes can include adding or removing specific modes, such as setting a channel to invite-only or giving operator status to a user. To view the current modes of a channel or user, the client can simply provide the target as a parameter, and the server will respond with the current modes associated with that target.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " MODE :Usage: /MODE <target> [modes]\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "CAP")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " CAP :** The CAP command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " CAP :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " CAP :The /CAP command is used to negotiate capabilities between the client and server. It allows the client to request specific features or extensions that the server may support.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " CAP :Usage: /CAP <subcommand> [arguments]\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "USER")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " USER :** The USER command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " USER :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " USER :The /USER command is used by a client to specify their username, hostname, servername, and real name when connecting to the IRC server. It is typically sent after the NICK command and is required for successful registration with the server.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " USER :Usage: /USER <username> <hostname> <servername> <realname>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    if (cmd == "PASS")
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + " PASS :** The PASS command **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PASS :\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + " PASS :The /PASS command is used by a client to provide a password for authentication when connecting to the IRC server. It is typically sent before the NICK and USER commands and is required if the server has password protection enabled.\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + " PASS :Usage: /PASS <password>\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    else
    {
        std::string errcmd = ":ircserver 704 " + sender.getName() + "* :** Help System **\r\n";
        errcmd += ":ircserver 705 " + sender.getName() + "* :\r\n";
        errcmd += ":ircserver 705 " + sender.getName() + "* :I do not know anything about this.\r\n";
        errcmd += ":ircserver 705 " + sender.getName() + "* :Try /HELP USERCMDS to list available\r\n";
        errcmd += ":ircserver 706 " + sender.getName() + "* :commands, or join the #help channel\r\n";
        sendResponse(sender.getFd(), errcmd);
        std::cout << "Client FD " << sender.getFd() << " requested help for unknown command: " << cmd << std::endl;
    }
}

// void Command::handleTOPIC(Client &sender, Server &server)
// {
//     std::string channelName = this->params[0][0];
//     std::string newTopic = this->params[1][0];
//     if (this->params.empty() || this->params[0].empty())
//     {
//         std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " TOPIC :Not enough parameters\r\n";
//         sendResponse(sender.getFd(), err);
//         std::cout << "Client FD " << sender.getFd() << " attempted to set topic without providing channel name." << std::endl;
//         return;
//     }
//     if (newTopic.empty())
//     {
//         std::string topic = server->findChannel(channelName)->getTopic();
//         if (!topic.empty())
//         {
//             std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + topic + "\r\n";
//             sendResponse(sender.getFd(), topicMsg);
//             std::cout << "Client FD " << sender.getFd() << " requested topic for channel " << channelName << ". Current topic: " << topic << std::endl;
//         }
//         else
//         {
//             std::string noTopicMsg = ":ircserver 331 " + sender.getName() + " " + channelName + " :No topic is set\r\n";
//             sendResponse(sender.getFd(), noTopicMsg);
//             std::cout << "Client FD " << sender.getFd() << " requested topic for channel " << channelName << ". No topic is set." << std::endl;
//         }
//         return;
//     }
//     bool mode = server->findChannel(channelName)->getTopicMode(); // do this to check if the channel is moderated or not
//     if (mode == true)
//     {
//         if (server->findChannel(channelName)->isOperator(&sender) == true)
//         {
//             server->findChannel(channelName)->setTopic(newTopic);
//             std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n";
//             sendResponse(sender.getFd(), topicMsg);
//             std::cout << "Client FD " << sender.getFd() << " set topic for channel " << channelName << " to: " << newTopic << std::endl;
//         }
//         else
//         {
//             std::string err = ":ircserver 482 " + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
//             sendResponse(sender.getFd(), err);
//             std::cout << "Client FD " << sender.getFd() << " attempted to set topic for channel " << channelName << " without operator privileges." << std::endl;
//         }
//     }
//     else
//     {
//         server->findChannel(channelName)->setTopic(newTopic);
//         std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n";
//         sendResponse(sender.getFd(), topicMsg);
//         std::cout << "Client FD " << sender.getFd() << " set topic for channel " << channelName << " to: " << newTopic << std::endl;
//     }
// }

void Command::handleCAP(Client &sender, Server &server)
{
    (void) server;
    if (this->params.empty())
    {
        std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " CAP :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to inquire capabilities without proper input(s)." << std::endl;
        return;
    }
    std::string subcommand = this->params[0][0];
    std::string commlist[] = {"LS", "LIST", "REQ", "END"};
    CapSubCommands  sub = UNKNOWN_CMD_CAP;
    for (size_t i = 0; i < 4; i++)
    {
        if (this->params[0][0] == commlist[i])
		{
			switch (i)
            {
                case 0:
                    sub = LS;
                    break;
                case 1:
                    sub = LIST;
                    break;
                case 2:
                    sub = REQ;
                    break;
                case 3:
                    sub = END;
                    break;
                
                default:
                    break;
            }
		}
    }
    if (sub == UNKNOWN_CMD_CAP)
    {
        std::string err = ":ircserver "+ intToString(ERR_INVALIDCAPCMD) + " " + sender.getName() + " CAP :Invalid CAP subcommands\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to inquire capabilities without proper subcommand." << std::endl;
        return;
    }
    switch (sub)
    {
        case LS:
            if (this->params[1].size() > 1 && !this->params[1].empty())
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() +  " LS "  + this->params[1][0] + " " + ":multi-prefix\r\n");
            else
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LS :multi-prefix\r\n");
            break;
        case LIST:
            // if (sender.hasMultiPrefixEnabled() == true) // add more function in client to get enable cap feature R+ P'Jay
            //     sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LIST :multi-prefix\r\n");
            // else
            //     sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LIST :\r\n");
            // break;
        case REQ:
            if (this->params[1].size() > 1 && this->params[2][0].find("multi-prefix") != std::string::npos)
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " ACK :multi-prefix\r\n");
            else
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " NAK :" + this->params[0][1] + "\r\n");
            break;
        case END:
            // add flag client end cap on client + set cap status negotiation R+ P'Jay
            std::cout << "Client FD " << sender.getFd() << "end CAP negotiations" << std::endl;
            break;
        
        default:
            break;
    }
}


// ERR_NEEDMOREPARAMS checked
// ERR_NOSUCHNICK checked
// ERR_NOSUCHCHANNEL checked
// ERR_USERONCHANNEL checked
// RPL_INVITING checked
void Command::handleINVITE(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty() || this->params.size() < 2 || this->params[1].empty())
    {
        std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " INVITE :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to invite without providing necessary parameters." << std::endl;        
        return;
    }
    std::string targetNick = this->params[0][0];
    std::string channelName = this->params[1][0];
    Client* targetClient = server.findClient(targetNick);
    Channel* targetChannel = server.findChannel(channelName);
    if (!targetClient)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + targetNick + " :No such nick\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to invite non-existent user: " << targetNick << std::endl;
        return;
    }
    if (!targetChannel)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " :No such channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to invite to non-existent channel: " << channelName << std::endl;
        return;
    }
    if (!targetChannel->hasClient(&sender))
    {
        std::string err = ":ircserver 442 " + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to invite user to channel they are not part of: " << channelName << std::endl;
        return;
    }
    if (targetChannel->hasClient(targetClient))
    {
        std::string err = ":ircserver " + intToString(ERR_USERONCHANNEL) + " " + sender.getName() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to invite user who is already on the channel: " << targetNick << " to channel: " << channelName << std::endl;
        return;
    }
    std::string inviteMsg = ":ircserver " + intToString(RPL_INVITING) + " " + sender.getName() + " " + targetNick + " :" + channelName + "\r\n";
    targetChannel->removeClient(targetClient); // remove the client from the channel's invite list if they were previously invited
    sendResponse(targetClient->getFd(), inviteMsg);
    std::cout << sender.getName() << " invited " << targetNick << " to join channel " << channelName << std::endl;
}


// ERR_NEEDMOREPARAMS checked
// ERR_NOSUCHNICK checked
// ERR_NOSUCHCHANNEL checked
// ERR_NOTONCHANNEL checked
// ERR_USERNOTONCHANNEL checked
void Command::handleKICK(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty() || this->params.size() < 2 || this->params[1].empty())
    {
        std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " KICK :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick without providing necessary parameters." << std::endl;        
        return;
    }
    std::string targetNick = this->params[0][0];
    std::string channelName = this->params[1][0];
    Client* targetClient = server.findClient(targetNick);
    Channel* targetChannel = server.findChannel(channelName);
    if (!targetClient)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + targetNick + " :No such nick\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick non-existent user: " << targetNick << std::endl;
        return;
    }
    if (!targetChannel)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " :No such channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick from non-existent channel: " << channelName << std::endl;
        return;
    }
    if (!targetChannel->hasClient(&sender))
    {
        std::string err = ":ircserver 442 " + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick user from channel they are not part of: " << channelName << std::endl;
        return;
    }
    if (!targetChannel->hasClient(targetClient))
    {
        std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + targetNick + " " + channelName + " :is not on channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick user who is not on the channel: " << targetNick << " from channel: " << channelName << std::endl;
        return;
    }
    std::string kickMsg = ":ircserver KICK " + channelName + " " + targetNick + " :Kicked by " + sender.getName() + "\r\n";
    targetChannel->broadcast(targetClient, kickMsg); // Notify the channel that the user has been kicked
    targetChannel->removeClient(targetClient); // Remove the client from the channel
    std::cout << sender.getName() << " kicked " << targetNick << " from channel " << channelName << std::endl;
    if (targetChannel->isEmpty())
    {
        std::cout << "Deleting the channel " << channelName << " as it is now empty after kick." << std::endl;
        server.deleteChannel(channelName);
    }
}