#include <iostream>
#include <string>

std::string	numeric_replies(const size_t code)
{
	switch (code)
	{
		case 001:
			return "Welcome to Custom Made Internet Relay Chat Project";
		case 002:
			return "";
		case 003:
			return "";
		case 004:
			return "";
		case 221:
			return "";
		case 263:
			return "An Error Occured, Please Try Again";
		case 331:
			return "Topic Not Found";
		case 332:
			return "Topic: ";
		case 333:
			return "";
		case 341:
			return "Inviting $USER to $CHANNEL";
		case 353:
			return "";
		case 366:
			return "";
		case 381:
			return "You are an Operator";
		case 401:
			return "No such Nickname";
		case 402:
			return "No such Server";
		case 403:
			return "No such Channel";
		case 404:
			return "Cannot Send to Channel $CHANNEL";
		case 405:
			return "You have joined too many channels";
		case 409:
			return "No Origin Specified";
		case 411:
			return "No Recipient Given";
		case 412:
			return "No Text to send";
		case 421:
			return "Unknown Command";
		case 431:
			return "No Nickname Given";
		case 432:
			return "This Nickname is Against This IRC Rules";
		case 433:
			return "This Nickname is Already in Use";
		case 436:
			return "";
		case 441:
			return "$NICK are not on the mentioned channel";
		case 442:
			return "You are not on the mentioned channel";
		case 443:
			return "$USER is on the channel";
		case 451:
			return "You have Not Registered";
		case 461:
			return "Insufficient Parameters";
		case 462:
			return "You are Already Registered";
		case 464:
			return "Incorrect Password";
		case 471:
			return "Channel is Full";
		case 473:
			return "This Channel $CHANNEL is joined by Invitation Only";
		case 474:
			return "You have been banned from this channel $CHANNEL";
		case 475:
			return "";
		case 476:
			return "";
		case 482:
			return "";
		case 491:
			return "This Channel has no Operator as Host";
		case 502:
			return "Username does not match";
		
		default:
			break;
	}
}
