/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 12:23:39 by jjaroens          #+#    #+#             */
/*   Updated: 2026/07/03 11:03:13 by gyeepach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

static std::string buildClientPrefix(const Client &sender)
{
	std::string username = sender.getUsername().empty() ? "unknown" : sender.getUsername();
	std::string host = sender.getIp().empty() ? "localhost" : sender.getIp();
	return ":" + sender.getName() + "!" + username + "@" + host;
}

Channel::Channel(){}
Channel::~Channel(){}

Channel::Channel(const std::string &name, const std::string &key, Client *client)
    :_name(name), _key(key), _limit(0), _inviteOnly(false),
    _topicRestrict(false), _hasKey(false), _hasLimited(false)
{
    _creationTime = std::time(NULL);
	addOperator(client->getFd());
}

std::string		Channel::getName() const
{
	return _name;
}

std::string		Channel::getKey() const
{
	return _key;
}

size_t	Channel::getLimit()
{
    return _limit;
}


size_t	Channel::getChannelSize()
{
	return _clients.size();
}

void	Channel::setKey(std::string key)
{
	_key = key;
}

void	Channel::setLimit(size_t limit)
{
	_limit = limit;
}

void	Channel::broadcast(Server &server, Client *sender, const std::string &message)
{
	for (unsigned long i = 0; i < _clients.size(); i++)
	{
		Client* target = _clients[i];
		if (target == sender)
			continue;
		target->appendWriteBuffer(message);
		server.enablePollOut(target->getFd());
	}
	this->_broadcast_buffer.clear();
}

void	Channel::mode_broadcast(const std::string &message)
{
	for (unsigned long i = 0; i < _clients.size(); i++)
	{
		Client* target = _clients[i];
		response(target->getFd(), message);
	} 
}

void	Channel::response(int fd, const std::string &msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

bool	Channel::checkKey(const std::string &key)
{
	if (_key.empty())
		return true;
	return _key == key;
}

void	Channel::addClient(Client *client)
{
	if (!client)
		return;
	if (hasClient(client))
		return;
	_clients.push_back(client);
	client->addChannel(this);
}

void	Channel::addInviteClient(Client *client)
{
	_invited_clients.push_back(client);
}

bool	Channel::hasClient(Client *client) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == client)
			return true;
	}
	return false;
}


void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::removeClient(Client *client)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == client)
		{
			_clients.erase(it);
			return;
		}
	}
}

void	Channel::removeOperator(Client *client)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it == client->getFd())
		{
			_operators.erase(it);
			return;
		}
	}
}

bool	Channel::isEmpty()
{
	return _clients.empty();
}

bool	Channel::isOperator(int fd)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i] == fd)
			return true;
	}
	return false;
}

void	Channel::addOperator(int fd)
{
	if (!isOperator(fd))
		_operators.push_back(fd);
}


void    Channel::broadcastModeChange(Client &sender, const std::string &modeChanges)
{
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	mode_broadcast(msg);
}

bool    Channel::checkOperator(Client &client)
{
	if (!isOperator(client.getFd()))
	{
		std::string err = 
		":ircserver 482 " + client.getName() +
		" " + _name + " :You're not channel operator\r\n";
		response(client.getFd(), err);
		return false;
	}
	return true;
}

// void    Channel::handleInviteMode(Client &sender, const std::string &modeChanges, Server &server)
// {
//     if (!checkOperator(sender))
//         return;

//     _inviteOnly = (modeChanges[0] == '+');

//     // 1. สร้างข้อความ
//     std::string msg = buildClientPrefix(sender) + " MODE " + _name + " " + modeChanges + "\r\n";
//     std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + (modeChanges[0] == '+' ? "+i" : "") + "\r\n";

//     // 2. ลูปที่ 1: ยัดลง Buffer ให้ครบทุกคน (เตรียมของให้พร้อม)
//     for (unsigned long i = 0; i < _clients.size(); i++) {
//         if (_clients[i] != NULL) {
//             _clients[i]->appendWriteBuffer(msg);
//             // ถ้าเป็นคนสั่ง ให้แปะ 324 ตามลงไปใน Buffer ทันที
//             if (_clients[i]->getFd() == sender.getFd()) {
//                 _clients[i]->appendWriteBuffer(rpl_324);
//             }
//         }
//     }

//     // 3. ลูปที่ 2: เปิด POLLOUT ให้ทุกคนพร้อมกัน (กดสวิตช์ส่งพร้อมกัน)
//     for (unsigned long i = 0; i < _clients.size(); i++) {
//         if (_clients[i] != NULL) {
//             server.enablePollOut(_clients[i]->getFd());
//         }
//     }
// }

void	Channel::handleInviteMode(Client &sender, const std::string &modeChanges, Server &server)
{
	if (!checkOperator(sender))
		return;
	if (modeChanges[0] == '+')
		_inviteOnly = true;
	else if (modeChanges[0] == '-')
		_inviteOnly = false;
	// broadcastModeChange(sender, modeChanges);
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	std::string current_modes = (_inviteOnly ? "+i" : "-i"); // เพิ่ม logic เช็ค mode อื่นได้ที่นี่
    std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + current_modes + "\r\n";
	std::string final_msg = msg + rpl_324;
	for (unsigned long i = 0; i < _clients.size(); i++) {
		if (_clients[i] != NULL) {
			_clients[i]->appendWriteBuffer(final_msg);
			server.enablePollOut(_clients[i]->getFd());
		}
	}
}

void	Channel::handleTopicMode(Client &sender, const std::string &modeChanges, Server &server)
{
	// if (!checkOperator(sender))
	// 	return;
	// if (modeChanges[0] == '+')
	// 	_topicRestrict = true;
	// else
	// 	_topicRestrict = false;
	// broadcastModeChange(sender, modeChanges);
	if (!checkOperator(sender))
		return;
	if (modeChanges[0] == '+')
		_topicRestrict = true;
	else if (modeChanges[0] == '-')
		_topicRestrict = false;
	// broadcastModeChange(sender, modeChanges);
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	std::string current_modes = (_topicRestrict ? "+t" : "-t"); // เพิ่ม logic เช็ค mode อื่นได้ที่นี่
    std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + current_modes + "\r\n";
	std::string final_msg = msg + rpl_324;
	for (unsigned long i = 0; i < _clients.size(); i++) {
		if (_clients[i] != NULL) {
			_clients[i]->appendWriteBuffer(final_msg);
			server.enablePollOut(_clients[i]->getFd());
		}
	}
}


void	Channel::handleKeyMode(Client &sender, const std::string &modeChanges, const std::string &param, Server &server)
{
	// if (!checkOperator(sender))
	// 	return;
	// if (modeChanges[0] == '+')
	// {
	// 	if (param.empty())
	// 	{
	// 		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
	// 		response(sender.getFd(), err);
	// 		// std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
	// 		return;
	// 	}
	// 	_hasKey = true;
	// 	_key = param;
	// }
	// else if (modeChanges[0] == '-')
	// {
	// 	_hasKey = false;
	// 	_key.clear();
	// }
	// else
	// {
	// 	std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
	// 	response(sender.getFd(), err);
		// std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
	// }
	// broadcastModeChange(sender, modeChanges);
	if (!checkOperator(sender))
		return;
	if (modeChanges[0] == '+')
	{
		if (param.empty())
		{
			std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
			sender.appendWriteBuffer(err);
			server.enablePollOut(sender.getFd());
			return ;
		}
		_hasKey = true;
		_key = param;
	}
	else if (modeChanges[0] == '-')
	{
		_hasKey = false;
		_key.clear();
	}
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	std::string current_modes = (_hasKey ? "+k" : "-k"); // เพิ่ม logic เช็ค mode อื่นได้ที่นี่
    std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + current_modes + "\r\n";
	std::string final_msg = msg + rpl_324;
	for (unsigned long i = 0; i < _clients.size(); i++) {
		if (_clients[i] != NULL) {
			_clients[i]->appendWriteBuffer(final_msg);
			server.enablePollOut(_clients[i]->getFd());
		}
	}
}

void	Channel::handleLimitMode(Client &sender, const std::string &modeChanges, const std::string &param, Server &server)
{
	// if (!checkOperator(sender))
	// 	return;
	// if (modeChanges[0] == '+')
	// {
	// 	if (param.empty())
	// 	{
	// 		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
	// 		response(sender.getFd(), err);
	// 		// std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
	// 		return;
	// 	}
	// 	_hasLimited = true;
	// 	_limit = std::atoi(param.c_str());
	// }
	// else if (modeChanges[0] == '-')
	// {
	// 	_hasLimited = false;
	// 	_limit = 0;
	// }
	// else
	// {
	// 	std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
	// 	response(sender.getFd(), err);
	// 	// std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
	// }
	// broadcastModeChange(sender, modeChanges);
	if (!checkOperator(sender))
		return;
	if (modeChanges[0] == '+')
	{
		if (param.empty())
		{
			std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
			sender.appendWriteBuffer(err);
			server.enablePollOut(sender.getFd());
			return;
		}
		_hasLimited = true;
		_limit = std::atoi(param.c_str());
	}
	else if (modeChanges[0] == '-')
	{
		_hasLimited =false;
		_limit = 0;
	}
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	std::string current_modes = (_hasLimited ? "+l" : "-l");
	std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + current_modes + "\r\n";
	std::string final_msg = msg + rpl_324;
	for (unsigned long i = 0; i < _clients.size(); i++) {
		if (_clients[i] != NULL) {
			_clients[i]->appendWriteBuffer(final_msg);
			server.enablePollOut(_clients[i]->getFd());
		}
	}
}

void	Channel::handleOperatorMode(Client &sender, const std::string &modeChanges, const std::string &nick, Server &server)
{
	// if (!checkOperator(sender))
	// 	return;
	// if (nick.empty())
	// {
	// 	std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
	// 	response(sender.getFd(), err);
	// 	// std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
	// 	return;
	// }
	// Client* target = server.findClient(nick);
	// if (!target)
	// {
	// 	std::string err = ":ircserver " + intToString(ERR_NOSUCHNICK) + " " + sender.getName() + " :No such nick\r\n";
	// 	response(sender.getFd(), err);
	// 	// std::cout << "Client FD " << sender.getFd() << " attempted to change modes for non-existent user: " << std::endl;
	// 	return;
	// }
	// if (modeChanges[0] == '+')
	// 	addOperator(target->getFd());
	// else if (modeChanges[0] == '-')
	// 	removeOperator(target);
	// else
	// {
	// 	std::string err = ":ircserver " + intToString(ERR_UMODEUNKOWNFLAG) + " " + sender.getName() + " " + modeChanges[1] + " :is unknown mode char\r\n";
	// 	response(sender.getFd(), err);
	// 	// std::cout << "Client FD " << sender.getFd() << " attempted to change modes with unknown mode character: " << modeChanges[1] << std::endl;
	// }
	// broadcastModeChange(sender, modeChanges);
	std::string current_modes = "";
	if (!checkOperator(sender))
		return;
	if (nick.empty())
	{
		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		// std::cout << "Client FD " << sender.getFd() << " attempted to change modes without specifying changes." << std::endl;
		return;
	}
	Client* target = server.findClient(nick);
	if (!target)
	{
		std::string err = ":ircserver " + intToString(ERR_NEEDMOREPARAMS) + " " + sender.getName() + " MODE :Not enough parameters\r\n";
		sender.appendWriteBuffer(err);
		server.enablePollOut(sender.getFd());
		return;
	}
	if (modeChanges[0] == '+')
	{
		addOperator(target->getFd());
		current_modes = "+o";
	}
	else if (modeChanges[0] == '-')
	{
		removeOperator(target);
		current_modes = "-o";
	}
	std::string msg = buildClientPrefix(sender) + " MODE " +
						_name + " " + modeChanges + "\r\n";
	std::string rpl_324 = ":ircserver 324 " + sender.getName() + " " + _name + " :" + current_modes + "\r\n";
	std::string final_msg = msg + rpl_324;
	for (unsigned long i = 0; i < _clients.size(); i++) {
	if (_clients[i] != NULL) {
		_clients[i]->appendWriteBuffer(final_msg);
		server.enablePollOut(_clients[i]->getFd());
	}
	}
}


void    Channel::setTopic(const std::string &topic, const std::string &setter)
{
	_topic = topic;
	_topicSetTime = std::time(NULL);
	_setter_topic = setter;
}

std::string	Channel::getTopic() const
{
	return _topic;
}


bool	Channel::getTopic_mode() const
{
	if (_topicRestrict)
		return true;
	else
		return false;
}


void	Channel::setInviteOnly(bool inviteOnly)
{
	_inviteOnly = inviteOnly;
}

bool	Channel::isInviteOnly() const
{
	return _inviteOnly;
}

bool	Channel::isInvited(Client *client) const
{
	for (std::vector<Client*>::const_iterator it = _invited_clients.begin(); it != _invited_clients.end(); it++)
	{
		if (*it == client)
			return true;
	}
	return false;
}

std::string	Channel::getsetter_topic() const
{
	return _setter_topic;
}

std::vector<Client *>	Channel::getClients()
{
	return _clients;
}

std::string	Channel::getCreationTimestr() const
{
	char buffer[80];
	std::tm *timeinfo = std::localtime(&_creationTime);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
	return std::string(buffer);
}


std::time_t	Channel::getCreationTime() const
{
	return _creationTime;
}

std::string	Channel::getCreationTimeStr_Topic() const
{
	char buffer[80];
	std::tm *timeinfo = std::localtime(&_creationTime);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
	return std::string(buffer);
}

std::time_t	Channel::getCreationTime_Topic() const
{
	return _topicSetTime;
}

size_t	Channel::get_operators_size() const
{
	return _operators.size();
}

std::string&	Channel::get_broadcast_buffer()
{
	return (_broadcast_buffer);
}


std::string Channel::append_buffer(std::string buffer)
{
		this->_broadcast_buffer = buffer;
		this->_broadcast_buffer = true;
		return (this->_broadcast_buffer);
}