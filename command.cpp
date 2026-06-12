/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:46:55 by codespace         #+#    #+#             */
/*   Updated: 2026/06/11 21:27:21 by gyeepach         ###   ########.fr       */
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
    std::cout << "Sending to FD " << fd << ": " << message << std::endl;
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
    // std::cout << "Sending to FD: " << sender.getFd() << std::endl;
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
		case CAP:     handleCAP(sender, server);     break;
		case USER:    handleUSER(sender, server);    break;
		case PASS:    handlePass(sender, server);    break;
		case NICK:    handleNick(sender, server);    break;
		case JOIN:    handleJOIN(server, sender);    break;
		case PRIVMSG: handlePRIVMSG(server, sender); break;
		case QUIT:    handleQuit(sender, server);    break;
		case MODE:    handleMODE(sender, server);    break;
		case HELP:    handleHELP(sender, server);    break;
		case PART:    handlePart(server, sender);    break;
		case INVITE:  handleINVITE(sender, server);  break;
		case KICK:    handleKICK(sender, server);    break;
		case TOPIC:   handleTOPIC(sender, server);   break;
		case PING:
    	{
            if (this->params.empty() || this->params[0].empty()) return;
            std::string token = this->params[0][0];
            std::string pongResponse = ":ircserver PONG ircserver :" + token + "\r\n";
            sendResponse(sender.getFd(), pongResponse);
            break;
    	}
		default:      break;
	}
    // switch (cmdType)
    // {
    //     case CAP:
    //         // std::cout << "Executing CAP..." << std::endl;
    //         handleCAP(sender, server);
	// 		if (!sender.isAuthenticated() && sender.isPassSet() && sender.isNickSet() && sender.isUserSet())
    //         {
    //             sender.setAuthenticated(true);
    //             std::cout << "Client FD " << sender.getFd() << " successfully registered via CAP END trigger." << std::endl;
    //             sendWelcomeMessage(server, sender);
    //         }
    //         break;
    //     case USER:
    //         // std::cout <<  "Executing USER..." << std::endl;
    //         handleUSER(sender, server);
    //         break;
    //     case PASS:
    //         // std::cout << "Executing PASS..." << std::endl;
    //         handlePass(sender, server);
    //         break;
    //     case NICK:
    //         handleNick(sender, server);
    //         // std::cout << "Executing NICK..." << std::endl;
    //         break;
    //     case JOIN:
    //         handleJOIN(server, sender);
    //         break;
    //     case PRIVMSG:
    //         handlePRIVMSG(server,sender);
    //         break;
    //     case QUIT:
    //         // std::cout << "Client quitting..." << std::endl;
    //         handleQuit(sender, server);
    //         break;
    //     case MODE:
    //         // std::cout << "Executing MODE..." << std::endl;
    //         handleMODE(sender, server);
    //         break;
    //     case HELP:
    //     {
    //         handleHELP(sender, server);
    //         break;
    //     }
    //     case PING:
    //     {
    //         if (this->params.empty() || this->params[0].empty()) return;
    //         std::string token = this->params[0][0]; // ดึงสิ่งที่ Client ส่งมา (เช่น ชื่อเซิร์ฟเวอร์หรือเลขสุ่ม)
    //         std::string pongResponse = ":ircserver PONG ircserver :" + token + "\r\n";
    //         sendResponse(sender.getFd(), pongResponse);
    //         break;
    //     }
    //     case PART:
    //     {
    //         handlePart(server, sender);
    //         break;
    //     }
    //     case INVITE:
    //     {
    //         handleINVITE(sender, server);
    //         break;
    //     }
    //     case KICK:
    //     {
    //         handleKICK(sender, server);
    //         break;
    //     }
    //     case TOPIC:
    //     {
    //         handleTOPIC(sender, server);
    //         break;
    //     }
    //     default:
    //         // std::cout << "Command " << this->params[0][0] << " not implementes" << std::endl;
    //         break;
    // }
	if (sender.isPassSet() == true && sender.isNickSet() == true && sender.isUserSet() == true && sender.isAuthenticated() == false)
    {
		std::cout << "--- SUCCESS: Client FD " << sender.getFd() << " registered properly! ---" << std::endl;
        sendWelcomeMessage(server, sender);
        sender.setAuthenticated(true);
    }
    if (this->type == 0)
    {
    //     // std::string err = ":ircserver" + std::to_string(UNKNOWN_CMD) + " " + sender.getNick() + " :Unknown command\r\n";
    //     // sendResponse(sender.getFd(), err);
    //     // std::cout << "Unknown command from client fd " << sender.getFd() << ": " << err << std::endl;
        return;                                                                                                                                         
    }

}
void Command::handleNick(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty() || this->params[0][0] == "")
    {
        std::string current_nick = sender.getName().empty() ? "unknown" : sender.getName();
        std::string err = ":ircserver " + intToString(ERR_NONICKNAMEGIVEN) + " " + current_nick + " :No nickname given\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }

    std::string newNick = this->params[0][0];

    while (!newNick.empty() && (newNick[newNick.length() - 1] == '\r' || newNick[newNick.length() - 1] == '\n' || newNick[newNick.length() - 1] == ' ')) {
        newNick.erase(newNick.length() - 1, 1);
    }

    if (server.findClient(newNick) != NULL && server.findClient(newNick)->getFd() != sender.getFd())
    {
        std::string current_nick = sender.getName().empty() ? "*" : sender.getName();
        if (current_nick.empty()) current_nick = "*";
        std::string err = ":ircserver " + intToString(ERR_NICKNAMEINUSE) + " " + current_nick + " " + newNick + " :Nickname is already in use\r\n";
        sendResponse(sender.getFd(), err);
        return; 
    }

    if (newNick.empty() || newNick[0] == '#' || newNick[0] == '&' || newNick[0] == '!' || newNick[0] == '+'
        || newNick[0] == '@' || newNick[0] == ':'  || newNick[0] == ' '
        || newNick.find(' ') != std::string::npos || newNick.find('\t') != std::string::npos 
        || newNick.find('\r') != std::string::npos || newNick.find('\n') != std::string::npos
        || (newNick[0] >= '0' && newNick[0] <= '9'))
    {
        std::string err = ":ircserver " + intToString(ERR_ERRONEUSNICKNAME) + " " + newNick + " :Erroneous nickname\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }


    std::string oldNick = sender.getName();
    sender.setNick(newNick);
    sender.setNickSet(true);
    std::cout << "Client FD " << sender.getFd() << " successfully set nick to " << newNick << std::endl;


    if (sender.isAuthenticated() && !oldNick.empty() && oldNick != newNick)
        std::string nick_change_msg = ":" + oldNick + " NICK " + newNick + "\r\n";
}

void Command::handlePass(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty())
        return;
    if (sender.isAuthenticated())
    {
        std::string err = ":ircserver " + intToString(ERR_ALREADYREGISTRED) + " " + sender.getName() + " :You may not register again\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to re-authenticate." << std::endl;
    }
    else if (this->params[0][0] == server.get_password())
    {
        sender.setPassSet(true);
        std::cout << "Client FD " << sender.getFd() << " authenticated successfully." << std::endl;
    }
    else
    {
        std::string err = ":ircserver " + intToString(ERR_PASSWDMISMATCH) + " " + sender.getName() + " :Password incorrect\r\n";  
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

void Command::handleUSER(Client &sender, Server &server)
{
    (void) server;
    const size_t userlen = 9; // <username> length limit (RFC 2812 recommends 9 characters for the username)
    if (this->params.empty() || this->params.size() != 4 || this->params[0].empty() || this->params[1].empty() || this->params[2].empty() || this->params[3].empty())
    {
        std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " USER :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    if (sender.isAuthenticated() || sender.isUserSet())
    {
        std::string err = ":ircserver " + intToString(ERR_ALREADYREGISTRED) + " " + sender.getName() + " :You may not register again\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to re-register." << std::endl;
        return;
    }
    std::string username = this->params[0][0];
    std::string hostname = this->params[1][0];
    std::string servername = this->params[2][0];
    std::string realname = this->params[3][0];
    if (username.length() > userlen)
        username = username.substr(0, userlen);
    if (hostname.length() > 63)
        hostname = hostname.substr(0, 63);
    if (servername.length() > 63)
        servername = servername.substr(0, 63);
    if (realname.length() > 128)
        realname = realname.substr(0, 128);
    sender.setUsername(username);
    std::cout << "Client FD " << sender.getFd() << " set username to " << username << std::endl;
    sender.setUserSet(true);
}

void Command::handlePRIVMSG(Server &server, Client &sender)
{
    //ERR 411
    if (this->params.empty() || this->params[0].empty())
    {
        std::string err = ":ircserver " + intToString(ERR_NORECIPIENT) + " " + sender.getName() + " PRIVMSG :No recipient given\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    //ERR 412
    if (params.size() < 2 || params[1].empty())
    {
        std::string err = ":ircserver " + intToString(ERR_NOTEXTTOSEND) + " " + sender.getName() + " PRIVMSG :No text to send\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    std::string target = this->params[0][0];
    std::string message = this->params[1][0]; // second parameter is the message
    //channel message
    std::cout << "The channel name is " << target << std::endl;
    if (target[0] == '#' )
    {
        Channel* channel = server.findChannel(target);
        if (!channel) //403
        {
            std::cout << "Channel not found" << std::endl;
            std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + target + " :No such channel\r\n";
            sendResponse(sender.getFd(), err);
            return;
        }
        if (!channel->hasClient(&sender)) //404
        {
            std::cout << "Sender does not belong to " << target << std::endl;
            std::string err = ":ircserver " + intToString(ERR_CANNOTSENDTOCHAN) + " " + sender.getName() + " " + target + " :Cannot send to channel\r\n";
            sendResponse(sender.getFd(), err);
            return;
        }
        std::string text = ":" + sender.getName() + " PRIVMSG " + target + " : " + message + "\r\n";
        channel->broadcast(&sender, text);
    }
    else 
    {
        //user message
        Client *target_client = server.findClient(target);
        if (!target_client) //401
        {
            std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + target + " :No such nick/channel\r\n";
            sendResponse(sender.getFd(), err);
            return;
        }
        std::string text = ":" + sender.getName() + " PRIVMSG " + target + " : " + message + "\r\n";
        send(target_client->getFd(), text.c_str(), text.size(), 0);   
    }
}

void Command::handleJOIN(Server &server, Client &sender)
{
    if (this->params.empty())
    {
        std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " JOIN :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    // size_t joined_count = 0;
    std::string key = "";
    std::string channel_name = this->params[0][0];
    //ERR_BADCHANMASK
    if (channel_name.empty() 
        || channel_name.length() > 50 
        || (channel_name[0] != '#' && channel_name[0] != '&') // ต้องขึ้นต้นด้วย # หรือ & เท่านั้น
        || channel_name.find(' ') != std::string::npos
        || channel_name.find(',') != std::string::npos
        || channel_name.find('\a') != std::string::npos
        || channel_name.find('\r') != std::string::npos
        || channel_name.find('\n') != std::string::npos)
    {
        std::string err = ":ircserver " + intToString(ERR_BADCHANMASK) + " " + sender.getName() + " " + channel_name + " :Bad channel mask\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to join channel with invalid name: " << channel_name << std::endl;
        return;
    }
    if (server.findChannel(channel_name) == NULL )
    {
        if (channel_name.empty() || channel_name[0] != '#')
        {
            std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channel_name + " :No such channel\r\n";
            sendResponse(sender.getFd(), err);
            std::cout << "Client FD " << sender.getFd() << " attempted to join non-existent channel: " << channel_name << std::endl;
            return;
        }
    }   
    if (sender.getNumChan() >= sender.getLimitChan())
    {
        std::string err = ":ircserver " + intToString(ERR_TOOMANYCHANNELS) + " " + sender.getName() + " " + channel_name + " :You have joined too many channels\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    
    //ERR_BADCHANNELKEY
    if (server.findChannel(channel_name) != NULL && server.findChannel(channel_name)->getKey() != "" && (this->params.size() <= 1 || this->params[1].empty() || !server.findChannel(channel_name)->checkKey(this->params[1][0])))
    {
        std::string err = ":ircserver " + intToString(ERR_BADCHANNELKEY) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+k)\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to join channel with incorrect key: " << channel_name << std::endl;
        return;
    }
    //ERR_CHANNELISFULL
    if (server.findChannel(channel_name) != NULL && server.findChannel(channel_name)->getLimit() > 0 && server.findChannel(channel_name)->getChannelSize() >= server.findChannel(channel_name)->getLimit())
    {
        std::string err = ":ircserver " + intToString(ERR_CHANNELISFULL) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+l)\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to join full channel: " << channel_name << std::endl;
        return;
    }
    //ERR_INVITEONLYCHAN
    if (server.findChannel(channel_name) != NULL && server.findChannel(channel_name)->isInviteOnly() && !server.findChannel(channel_name)->isInvited(&sender))
    {
        std::string err = ":ircserver " + intToString(ERR_INVITEONLYCHAN) + " " + sender.getName() + " " + channel_name + " :Cannot join channel (+i)\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to join invite-only channel without an invitation: " << channel_name << std::endl;
        return;
    }
    if (this->params.size() > 1 && !this->params[1].empty())
        key = this->params[1][0];
    std::cout << "Channel name is: " << "\"" << channel_name << "\"" << " key is " << "\"" << key << "\"" << std::endl;
    server.findOrCreateChannel(channel_name, key, &sender);
    // RPL_TOPIC, RPL_TOPICWHOTIME, RPL_NAMREPLY, RPL_ENDOFNAMES
    // RPL_TOPIC :ircserver 332 <nick> <channel> :<topic>
    std::string topic = server.findChannel(channel_name)->getTopic();
    if (!topic.empty())
    {
        std::string topic_msg = ":ircserver " + intToString(RPL_TOPIC) + " " + sender.getName() + " " + channel_name + " :" + topic + "\r\n";
        sendResponse(sender.getFd(), topic_msg);
        std::string setter = server.findChannel(channel_name)->getsetter_topic();
        std::string topic_time_msg = ":ircserver " + intToString(RPL_TOPICWHOTIME) + " " + sender.getName() + " " + channel_name + " " + setter + " :Topic set time" + server.findChannel(channel_name)->getCreationTimeStr_Topic() + "\r\n";
        sendResponse(sender.getFd(), topic_time_msg);
    }
    std::string names_msg = ":ircserver " + intToString(RPL_NAMREPLY) + " " + sender.getName() + " = " + channel_name + " :";
    Channel* channel = server.findChannel(channel_name);
    std::vector<Client*> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (channel->isOperator(clients[i]->getFd()))
            names_msg += "@" + clients[i]->getName() + " ";
        else
            names_msg += clients[i]->getName() + " ";
    }
    names_msg += "\r\n";
    sendResponse(sender.getFd(), names_msg);
    std::string end_of_names_msg = ":ircserver " + intToString(RPL_ENDOFNAMES) + " " + sender.getName() + " " + channel_name + " :End of NAMES list\r\n";
    sendResponse(sender.getFd(), end_of_names_msg);
}

void    Command::handlePart(Server &server, Client &sender)
{
    if (this->params.empty() || this->params[0].empty())
    {
        std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() +
            "PART :Not enough parameters\r\n";
        
        sendResponse(sender.getFd(), err);
        return;
    }
    std::string channel_name = this->params[0][0];
    Channel* channel = server.findChannel(channel_name);
    if (!channel)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channel_name + " :No such channel\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    if (!channel->hasClient(&sender))
    {
        std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + channel_name + " :You're not on that channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client does not belong to the channgetCreationDateel " << channel_name << std::endl;
        return;
    }
    std::string message = ":" + sender.getName() + " PART " + channel_name + "\r\n";
    channel->broadcast(&sender, message);
    sendResponse(sender.getFd(), message);
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
    // MODE requires at least a target
    if (this->params.size() < 1 || this->params[0].empty())
    {
        std::string err = ":ircserver " +
        intToString(ERR_NEEDMOREPARAMS) + " " +
        sender.getName() +
        " MODE :Not enough parameters\r\n";
        
        sendResponse(sender.getFd(), err);
        return;
    }
    
    std::string modeTarget = this->params[0][0];
    
    // User mode handling not implemented
    if (modeTarget.empty() || modeTarget[0] != '#')
        return;
    
    Channel *channel = server.findChannel(modeTarget);
    if (!channel)
    {
        std::string err = ":ircserver " +
        intToString(ERR_NOSUCHCHANNEL) + " " +
        sender.getName() + " " +
        modeTarget +
        " :No such channel\r\n";
        
        sendResponse(sender.getFd(), err);
        return;
    }
    if (channel->isOperator(sender.getFd()) == false)
    {
        std::string err = ":ircserver " +
            intToString(ERR_CHANOPRIVSNEEDED) + " " +
            sender.getName() + " " +
            modeTarget +
            " :You're not channel operator\r\n";

        sendResponse(sender.getFd(), err);
        return;
    }

    // MODE #channel
    if (this->params.size() < 2 || this->params[1].empty())
    {
        std::string servername = "ircserver";
        std::string nick = sender.getName();
        std::string modestring = "+";
        std::string mode_args = "";

        if (channel->isInviteOnly())
            modestring += "i";
        if (channel->getKey() != "")
            modestring += "k";
        if (channel->getLimit() > 0)
            modestring += "l";
        if (channel->getTopic() != "") // getTopice in boolean not string
            modestring += "t";
        std::string rpl_mode_324 = ":ircserver " + intToString(RPL_CHANNELMODEIS) + " " + nick + " " + channel->getName() + " :" + modestring + mode_args + "\r\n";
        sendResponse(sender.getFd(), rpl_mode_324);
        std::string time = channel->getCreationTimestr();
        std::string rpl_mode_329 = ":ircserver " + intToString(RPL_CREATIONTIME) + " " + nick + " " + channel->getName() + " " + time + "\r\n";
        sendResponse(sender.getFd(), rpl_mode_329);
        return;
    }

    std::string modeChanges = this->params[1][0];

    if (modeChanges.empty())
    {
        std::string err = ":ircserver " +
            intToString(ERR_NEEDMOREPARAMS) + " " +
            sender.getName() +
            " MODE :Not enough parameters\r\n";

        sendResponse(sender.getFd(), err);
        return;
    }

    if (modeChanges[0] != '+' && modeChanges[0] != '-')
    {
        std::string err = ":ircserver " +
            intToString(ERR_UMODEUNKOWNFLAG) + " " +
            sender.getName() + " " +
            modeChanges +
            " :Unknown MODE flag\r\n";

        sendResponse(sender.getFd(), err);
        return;
    }

    // Need at least "+i", "-k", etc.
    if (modeChanges.length() < 2)
    {
        std::string err = ":ircserver " +
            intToString(ERR_UMODEUNKOWNFLAG) + " " +
            sender.getName() +
            " :Unknown MODE flag\r\n";

        sendResponse(sender.getFd(), err);
        return;
    }

    switch (modeChanges[1])
    {
        case 'k':
        {
            if (this->params.size() < 3 || this->params[2].empty())
            {
                std::string err = ":ircserver " +
                    intToString(ERR_NEEDMOREPARAMS) + " " +
                    sender.getName() +
                    " MODE :Not enough parameters\r\n";

                sendResponse(sender.getFd(), err);
                return;
            }

            channel->handleKeyMode(
                sender,
                modeChanges,
                this->params[2][0]);
            break;
        }

        case 'l':
        {
            if ((this->params.size() < 2 && modeChanges[0] == '-') || (this->params.size() < 3 && modeChanges[0] == '+'))
            {
                std::string err = ":ircserver " +
                    intToString(ERR_NEEDMOREPARAMS) + " " +
                    sender.getName() +
                    " MODE :Not enough parameters\r\n";
                sendResponse(sender.getFd(), err);
                return;
            }
            channel->handleLimitMode(
                sender,
                modeChanges,
                this->params[2][0]);
            break;
        }

        case 'o':
        {
            if (this->params.size() < 3 || this->params[2].empty())
            {
                std::string err = ":ircserver " +
                    intToString(ERR_NEEDMOREPARAMS) + " " +
                    sender.getName() +
                    " MODE :Not enough parameters\r\n";

                sendResponse(sender.getFd(), err);
                return;
            }

            channel->handleOperatorMode(
                sender,
                modeChanges,
                this->params[2][0],
                server);
            break;
        }

        case 't':
            channel->handleTopicMode(sender, modeChanges);
            break;

        case 'i':
            channel->handleInviteMode(sender, modeChanges);
            break;

        default:
        {
            std::string err = ":ircserver " +
                intToString(ERR_UMODEUNKOWNFLAG) + " " +
                sender.getName() + " " +
                std::string(1, modeChanges[1]) +
                " :is unknown mode char\r\n";

            sendResponse(sender.getFd(), err);
            return;
        }
    }
}

// void Command::handleMODE(Client &sender, Server &server)
// {
//     std::string modeTarget = this->params[0][0]; // The target of the MODE command (channel or user)
//     std::string modeChanges = this->params[1][0]; // The mode changes
//     bool client_or_channel = (modeTarget[0] == '#'); // Determine if the target is a channel or a user based on the first character
//     if (client_or_channel)
//     {
//         if (modeChanges.empty())
//         {
//             std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
//             sendResponse(sender.getFd(), err);
//             std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
//             return;
//         }
//         if (modeChanges[0] != '+' && modeChanges[0] != '-')
//         {
//             std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[0] + " :Unknown MODE flag\r\n";
//             sendResponse(sender.getFd(), err);
//             std::cout << "Client FD " << sender.getFd() << " attempted to change modes with invalid mode string: " << modeChanges << std::endl;
//             return;
//         }
//         // Handle channel mode changes
//         Channel* channel = server.findChannel(modeTarget);
//         if (channel)
//         {
//             switch (modeChanges[1]) // Example: Check the second character for specific mode flags
//             {
//                 case 'k': // Channel key
//                     std::cout << "Handling channel key mode change for channel: " << modeTarget << std::endl;
//                     channel->handleKeyMode(sender, modeChanges, this->params[2][0]); //third argument
//                     break;
//                 case 'l': // User limit
//                     std::cout << "Handling user limit mode change for channel: " << modeTarget << std::endl;
//                     channel->handleLimitMode(sender, modeChanges, this->params[2][0]);
//                     break;
//                 case 't': // restrictions of the TOPIC channel
//                     std::cout << "Handling t channel mode change for channel: " << modeTarget << std::endl;
//                     server.findChannel(modeTarget)->handleTopicMode(sender, modeChanges);
//                     break;
// 				case 'o': // operator given mode
// 					std::cout << "Handling operator given mode change for channel: " << modeTarget << std::endl;
//                     channel->handleOperatorMode(sender, modeChanges, this->params[2][0], server); //third argument is the nick of the user to be opped/deopped
// 					break;
// 				case 'i': // invite-only mode
// 					std:: cout << "Handling Invite only channel mode change for channel: " << modeTarget << std::endl;
//                     channel->handleInviteMode(sender, modeChanges);
// 					break;
//                 default:
//                     std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
//                     sendResponse(sender.getFd(), err);
//                     std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
//                     return;
//             }
//             // Here you would implement the logic to change the channel modes based on modeChanges
//         }
//         else
//         {
//             std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + modeTarget + " :No such channel\r\n";
//             sendResponse(sender.getFd(), err);
//             std::cout << "Client FD " << sender.getFd() << " attempted to change modes for non-existent channel: " << modeTarget << std::endl;
//         }
//     }
//     else
// 		return ;
// }

// These are function not testing yet, so I will implement them later after I finish the main functions of the server and client

void Command::handleHELP(Client &sender, Server &server)
{
    (void) server;
    
    
    if (this->params.empty() || this->params[0][0].empty())
    {
        std::string helpMsg = ":ircserver 704" + sender.getName() + "* :** Help System **\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + "* :Available commands: JOIN, PART, PING, NICK, TOPIC, INVITE, KICK, PRIVMSG, MODE, CAP, USER, PASS\r\n";
        helpMsg += ":ircserver 705 " + sender.getName() + "* :/HELP USERCMDS to list available\r\n";
        helpMsg += ":ircserver 706 " + sender.getName() + "* :commands, or join the #help channel\r\n";
        sendResponse(sender.getFd(), helpMsg);
        return;
    }
    std::string cmd = this->params[0][0];
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

void Command::handleTOPIC(Client &sender, Server &server)
{
	if (this->params.empty() || this->params[0].empty() || this->params[1].empty())
	{
		std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " TOPIC :Not enough parameters\r\n";
		sendResponse(sender.getFd(), err);
		std::cout << "Client FD " << sender.getFd() << " attempted to set topic without providing channel name." << std::endl;
		return;
	}
	std::string channelName = this->params[0][0];
	if (server.findChannel(channelName) == NULL)
	{
		std::cout << "Channel not found" << std::endl;
		std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " :No such channel\r\n";
		sendResponse(sender.getFd(), err);
		return;
	}
    Channel *target_channel = server.findChannel(channelName);
    if (!target_channel->hasClient(&sender))
    {
        std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + " " + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to set topic for channel " << channelName << " without being a member." << std::endl;
        return;
    }
	if (this->params.size() < 2 || this->params[1].empty() || this->params[1][0] == "")
	{
		std::string topic = target_channel->getTopic();
		if (!topic.empty())
		{
			std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + topic + "\r\n";
			sendResponse(sender.getFd(), topicMsg);
			std::cout << "Client FD " << sender.getFd() << " requested topic for channel " << channelName << ". Current topic: " << topic << std::endl;
            std::string setter = target_channel->getsetter_topic();
            std::string topic_time_msg = ":ircserver 333 " + sender.getName() + " " + channelName + " " + setter + " :Topic set time" + target_channel->getCreationTimeStr_Topic() + "\r\n";
            sendResponse(sender.getFd(), topic_time_msg);
		}
		else
		{
			std::string noTopicMsg = ":ircserver 331 " + sender.getName() + " " + channelName + " :No topic is set\r\n";
			sendResponse(sender.getFd(), noTopicMsg);
			std::cout << "Client FD " << sender.getFd() << " requested topic for channel " << channelName << ". No topic is set." << std::endl;
		}
		return;
	}
	std::string newTopic = this->params[1][0];
	bool mode = target_channel->getTopic_mode(); // do this to check if the channel is moderated or not
	if (mode == true && target_channel->isOperator(sender.getFd()) == false)
	{
        std::string err = ":ircserver " + intToString(ERR_CHANOPRIVSNEEDED) + " " + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to set topic without operator privileges." << std::endl;
        return;
    }
    else if (mode == true && target_channel->isOperator(sender.getFd()) == true)
    {
        target_channel->setTopic(newTopic, sender.getName()); // Set the new topic with the name of the user who set it
        target_channel->broadcast(&sender, ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n");
        std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n";
        sendResponse(sender.getFd(), topicMsg);
        std::cout << "Client FD " << sender.getFd() << " set topic for channel " << channelName << " to: " << newTopic << std::endl;
    }
    else if  (mode == false)
    {
        target_channel->setTopic(newTopic, sender.getName()); // Set the new topic with the name of the user who set it
        target_channel->broadcast(&sender, ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n");
        std::string topicMsg = ":ircserver 332 " + sender.getName() + " " + channelName + " :" + newTopic + "\r\n";
        sendResponse(sender.getFd(), topicMsg);
        std::cout << "Client FD " << sender.getFd() << " set topic for channel " << channelName << " to: " << newTopic << std::endl;
	}
}

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
            sender.setCapNegotiating(true); // set a flag in client to indicate that CAP negotiation is in progress
            if (this->params.size() > 1 && !this->params[1].empty()) // leak solved checked
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() +  " LS "  + this->params[1][0] + " " + " multi-prefix\r\n");
            else
                sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LS multi-prefix\r\n");
            break;
        case LIST:
            // if (sender.hasMultiPrefixEnabled() == true) // add more function in client to get enable cap feature R+ P'Jay
            //     sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LIST :multi-prefix\r\n");
            // else
            //     sendResponse(sender.getFd(), ":ircserver CAP " + sender.getName() + " LIST :\r\n");
            // break;
        case REQ:
        {
            bool has_multi_prefix = false;
            for (size_t i = 1; i < this->params.size(); ++i)
            {
                if (!this->params[i].empty())
                {
                    // ตรวจค้นคำว่า "multi-prefix" (วิธีนี้ต่อให้ติดเครื่องหมาย : หรือเว้นวรรคแปลกๆ ก็เอาอยู่)
                    if (this->params[i][0].find("multi-prefix") != std::string::npos)
                    {
                        has_multi_prefix = true;
                        break;
                    }
                }
            }
            if (has_multi_prefix)
            {
                // ตอบรับ (ACK) ด้วยรูปแบบที่ Irssi ต้องการ
                // สำคัญมาก: บางไคลเอนต์ต้องการให้ส่งเครื่องหมาย : กลับไปด้วย เช่น :multi-prefix
                std::string reply = ":ircserver CAP " + sender.getName() + " ACK :multi-prefix\r\n";
                sendResponse(sender.getFd(), reply);
            }
            else
            {
                // ถ้าเป็นฟีเจอร์อื่นที่เราไม่รองรับ ให้ส่ง NAK
                // โดยการส่งค่าที่เขาส่งมากลับไปปฏิเสธ
                std::string requested = (this->params.size() > 1 && !this->params[1].empty()) ? this->params[1][0] : "unknown";
                std::string reply = ":ircserver CAP " + sender.getName() + " NAK :" + requested + "\r\n";
                sendResponse(sender.getFd(), reply);
            }
            break;
        }
        case END:
            sender.setCap(true); // set a flag in client to indicate that CAP negotiation is complete and the client can now use the enabled capabilities
            sender.setCapNegotiating(false); // reset the negotiating flag
            // sendWelcomeMessage(server, sender);
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


void Command::handleKICK(Client &sender, Server &server)
{
    if (this->params.empty() || this->params[0].empty() || this->params.size() < 2 || this->params[1].empty())
    {
        std::string err = ":ircserver "+ intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " KICK :Not enough parameters\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick without providing necessary parameters." << std::endl;        
        return;
    }
    std::string channelName = this->params[0][0];
    std::string targetNick = this->params[1][0];
    // check error ERR_BADCHANMASK
    if (channelName[0] != '#' && channelName[0] != '&') // check if the channel name starts with # or & which are valid channel prefixes in IRC
    {
        std::string err = ":ircserver " + intToString(ERR_BADCHANMASK) + " " + sender.getName() + " KICK :Bad channel mask\r\n";    
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick with invalid parameters: targetNick: " << targetNick << ", channelName: " << channelName << std::endl;
        return;
    }
    Client* targetClient = server.findClient(targetNick);
    Channel* targetChannel = server.findChannel(channelName);
    // Check first if Channel exit
    if (!targetChannel)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHCHANNEL) + " " + sender.getName() + " " + channelName + " : no such channel\r\n";
        sendResponse(sender.getFd(), err);
        return;
    }
    if (!targetClient)
    {
        std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " " + targetNick + " :No such nick\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick non-existent user: " << targetNick << std::endl;
        return;
    }
    if (!targetChannel->hasClient(&sender))
    {
        std::string err = ":ircserver " + intToString(ERR_NOTONCHANNEL) + sender.getName() + " " + channelName + " :You're not on that channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick user from channel they are not part of: " << channelName << std::endl;
        return;
    }
    if (!targetChannel->hasClient(targetClient))
    {
        std::string err = ":ircserver " + intToString(ERR_USERNOTINCHANNEL) + " " + sender.getName() + " " + targetNick + " " + channelName + " :is not on channel\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick user who is not on the channel: " << targetNick << " from channel: " << channelName << std::endl;
        return;
    }
    if (targetChannel->isOperator(sender.getFd()) == false)
    {
        std::string err = ":ircserver " + intToString(ERR_CHANOPRIVSNEEDED) + sender.getName() + " " + channelName + " :You're not channel operator\r\n";
        sendResponse(sender.getFd(), err);
        std::cout << "Client FD " << sender.getFd() << " attempted to kick user from channel without operator privileges: " << channelName << std::endl;
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

void sendWelcomeMessage(Server &server, Client &sender)
{
    if (sender.isNickSet() && sender.isUserSet() && sender.isPassSet() && !sender.isAuthenticated())
    {
        // sender.setAuthenticated(true);
        std::cout << "Client FD " << sender.getFd() << " has successfully registered." << std::endl;
        std::string clientNick = sender.getName();
        std::string welcomeMsg = ":ircserver 001 " + clientNick + " :Welcome to the IRC server, " + clientNick + "!\r\n";
        welcomeMsg += ":ircserver 002 " + clientNick + " :Your host is ircserver, running version 1.0\r\n";
        welcomeMsg += ":ircserver 003 " + clientNick + " :This server was created on " + server.get_creation_date() + "\r\n";
        welcomeMsg += ":ircserver 004 " + clientNick + " ircserver 1.0 i tkolk\r\n";
        
        sendResponse(sender.getFd(), welcomeMsg);
    }
}

bool isnumeric(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}