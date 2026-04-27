#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <cctype>
#include "./include/Client.hpp"
#include "./include/Server.hpp"

#define SUCCESS 0
#define FAILURE 1

class Client;

enum CommandPrompts
{
	UNKNOWN_CMD,
	CAP,
	CONNECT,
	QUIT, // To do
	JOIN, // To do
	PART, // To do
	PING, // TO do
	HELP,
	NICK, // Done
	PASS, // To do
	TOPIC,
	INVITE,
	KICK,
	PRIVMSG, // To do
	OPER,
	MODE,
	RESTART
};


// enum CommandPrompts
// {
// 	CAP,
// 	CONNECT,
// 	QUIT,
// 	JOIN,
// 	PART,
// 	PING,
// 	HELP,
// 	NICK,
// 	PASS,
// 	TOPIC, // OPER?? AI answer me please!!!
// 		// TOPIC is a command that allows users to set or view the topic of a channel. It is not specific to operators, but regular users can also use it if they have the necessary permissions in the channel.
// 	INVITE, // OPER?? AI answer me please!!!
// 		// INVITE is a command that allows users to invite other users to a channel. It is not specific to operators, but regular users can also use it if they have the necessary permissions in the channel.
// 	KICK, // OPER ?? AI answer me please!!!
// 		// KICK is a command that allows users to remove other users from a channel. It is typically restricted to channel operators, so it is more likely to be an operator command.
// 	PRIVMSG,
// 	OPER, // OPER ?? AI answer me please!!!
// 		// OPER is a command that allows users to gain operator status on the server. It is specific to operators, so it is definitely an operator command.
// 	MODE, // OPER ?? AI answer me please!!!
// 		// MODE is a command that allows users to change the mode of a channel or user. It can be used by both regular users and operators, depending on the specific mode being changed and the permissions of the user.
// 	RESTART // OPER ?? AI answer me please!!!
// 		// RESTART is a command that allows users to restart the server. It is typically restricted to server operators, so it is more likely to be an operator command.
// 	// OK AI all of these commands which are OPER are operator commands?
// 	// Yes, the commands that are typically associated with operator privileges (such as OPER, KICK, RESTART) are considered operator commands. However, some commands like MODE can be used by both regular users and operators depending on the context and permissions. It's important to check the specific server's implementation and permissions to determine which commands are operator-only.
// 	// In general, commands that allow users to manage channels, kick users, or perform administrative tasks are often restricted to operators, while commands that allow users to interact with the server or other users (like NICK, JOIN, PRIVMSG) are usually available to all users.
// 	// So, in summary:
// 	// - CAP, CONNECT, QUIT, JOIN, PART, PING, HELP, NICK, PASS: Regular user commands
// 	// - TOPIC, INVITE, MODE: Can be used by both regular users and operators depending on permissions
// 	// - KICK, OPER, RESTART: Typically operator commands
// 	// ok thank you
// 	// So in your implementation, you can categorize the commands based on their typical usage and permissions. You can also implement checks in your command handling logic to ensure that only users with the appropriate privileges can execute certain commands.
// 	// For example, when handling a command, you can check if the user is an operator before allowing them to execute commands like KICK or RESTART. For commands that can be used by both regular users and operators, you can check the user's permissions and handle the command accordingly.
// 	// Remember that the specific implementation and permissions may vary based on your server's design and requirements, so it's important to define your own rules and checks for command execution based on your server's needs.
// 	// I hope this helps you in categorizing and implementing the commands for your IRC server! If you have any more questions or need further assistance, feel free to ask.
// 	// ok thank you
// 	// I have a question how to implement OPER command it should be in execution function or not??
// 	// The OPER command is typically an operator command that allows users to gain operator status on the server. It should be implemented in the execution function of your command handling logic, as it is a specific command that requires certain checks and actions to be performed when executed.
// 	// When a user executes the OPER command, you would typically check if they have the necessary credentials (such as a password) to become an operator. If the credentials are valid, you would then grant them operator status and allow them to execute operator commands.
// 	// Here's a simplified example of how you might implement the OPER command in your execution function:
// 	// void Command::execute_command(Client &sender) {
// 	//     switch (cmdType) {
// 	//         case OPER:
// 	//             // Check if the sender has provided the correct credentials
// 	//             if (checkCredentials(sender)) {
// 	//                 // Grant operator status to the sender
// 	//                 sender.setOperator(true);
// 	//                 std::cout << "Client FD " << sender.getFd() << " is
// 	// now an operator." << std::endl;
// 	//             } else {
// 	//                 std::cout << "Client FD " << sender.getFd() << " failed
// 	// to become an operator due to invalid credentials." << std::endl;
// 	//             }
// 	//             break;
// 	//         // Handle other commands...
// 	//     }
// 	// }
// 	// ok thank you
// 	// You're welcome! If you have any more questions or need further assistance with implementing the OPER command or any other aspect of your IRC server, feel free to ask. Good luck with your implementation!
// };

class Command
{
	private:
		// std::string	command;
		size_t		type;
		// typedef void (Command::*Handler)(const IRCMessage&, Client&);
		// std::map<std::string, Handler> _hanlers;
		std::vector<std::vector<std::string> > params;
	public:
		Command();
		~Command();
		// void intitHandlers();
		void execute_command(Client &sender);
		void		msgparser(const std::string input);
		std::string	commandcheck(const std::string input);
		size_t	lexer(const std::string& input, const std::string& icomm, std::vector<std::vector<std::string> >& output);
		void handleNick(Client &sender);
		// void handlePass(Client &sender);
		// void handleQuit(Client &sender);
		// void handlePRIVMSG(Client &sender);
		// void handleJOIN(Client &sender);
};


enum NumRpl
{
	NONEED,
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
	RPL_UMODEIS = 221,
	RPL_TRYAGAIN = 263,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_YOUREOPER = 381,
	ERR_NOSUCHNICK = 401, 
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVILEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476, //optional
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOOPERHOST = 491,
	ERR_USERSDONTMATCH = 502,
//	.
//	.
//	.
};

// class EnviVar
// {
// 	private:
// 		char	**envmod;
// 		EnviVar();
// 	public:
// 		~EnviVar();
// 		char	**envparser(const char **env);
// };

// enum ConnectMess
// {
// 	CAP,
// 	AUTHENTICATE,
// 	PASS,
// 	NICK,
// 	USER,
// 	PING,
// 	PONG,
// 	OPER,
// 	QUIT,
// 	ERROR
// };
// enum ChannelMess
// {
// 	JOIN,
// 	PART,
// 	TOPIC,
// 	NAMES,
// 	LIST,
// 	INVITE,
// 	KICK
// };
// enum ServMess
// {
// 	MOTD,
// 	VERSION,
// 	ADMIN,
// 	CONNECT,
// 	LUSERS,
// 	TIME,
// 	STATS,
// 	HELP,
// 	INFO,
// 	MODE
// };
// enum UserMess
// {
// 	PRIVMSG,
// 	NOTICE,
// 	WHO,
// 	WHOIS,
// 	WHOWAS
// };
// enum OperMess
// {
// 	KILL,
// 	REHASH,
// 	RESTART,
// 	SQUIT
// };


#endif