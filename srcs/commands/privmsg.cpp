/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:40:14 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:25:19 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// syntax: PRIVMSG <target>{,<target>} <text to be sent>

void _privmsgProcess(std::string target, std::string message, User * user, Server * srv);
bool checkifexist(std::vector<std::string> args, User *user);

void Command::_privmsg(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());
	
	if (args.size() == 1)
		return (ERR_NORECIPIENT(user, "PRIVMSG"), void());
	if (args.size() == 2)
		return (ERR_NOTEXTTOSEND(user), void());
	//toomany targs 

	std::string message = "";
	if ( (args.size() > 2) && (args[2][0] == ':') )
	{
		message = args[2].substr(1);
		for (size_t i = 3; i < args.size(); i++)
			message.append(" " + args[i]);
	}

	if (message.empty())
		return (ERR_NOTEXTTOSEND(user), void());

	std::vector<std::string> targets = splitString(args[1]);

	for (size_t i = 0; i <targets.size(); i++)
		_privmsgProcess(targets[i], message, user, this->_srv);

}

void _privmsgProcess(std::string target, std::string message, User * user, Server * srv)
{
	if (target[0] == '#')
	{
		Channel * channel = srv->getChannel(target);
		if (channel == NULL)
			return (ERR_NOSUCHNICK(user, target), void());

		if (channel->getUser(user->getNickname()) == NULL)
			return (ERR_NOTONCHANNEL(user, target), void());

		channel->broadcastIgnore(RPL_PRIVMSG(user->getPrefix(), channel->get_name(), message), user);
	}
	else
	{
		User * targetedUser = srv->getUserByNickname(target);
		if (targetedUser == NULL)
			return (ERR_NOSUCHNICK(user, target), void());
		
		targetedUser->write(RPL_PRIVMSG(user->getPrefix(), user->getNickname(), message));
	}
}
