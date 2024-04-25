/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:45:06 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:23:09 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: KICK <channel> <user> *( "," <user> ) [<reason>]

void	_kickProcess(std::string target, Channel * channel, User * user, std::string name, std::string reason, Server * srv);

void	Command::_kick(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 3)
		return (ERR_NEEDMOREPARAMS(user, "KICK"), void());

	std::string name = args[1];

	std::string reason = "No particular reason";
	if ( (args.size() > 3) && (args[3][0] == ':') )
	{
		reason = args[3].substr(1);
		for (size_t i = 4; i < args.size(); i++)
			reason.append(" " + args[i]);
	}

	Channel * channel = this->_srv->getChannel(name);
	
	if (channel == NULL)
		return (ERR_NOSUCHCHANNEL(user, name), void());
	
	if ( channel->getUser(user->getNickname()) == NULL )
		return (ERR_NOTONCHANNEL(user, name), void());
	
	if (channel->isUserOperator(user) == false)
        return (ERR_CHANOPRIVSNEEDED(user, name), void());

	std::vector<std::string> targets = splitString(args[2]);

	for (size_t i = 0; i <targets.size(); i++)
		_kickProcess(targets[i], channel, user, name, reason, this->_srv);
}


void	_kickProcess(std::string target, Channel * channel, User * user, std::string name, std::string reason, Server * srv)
{
	User * targetedUser = channel->getUser(target);
	if (targetedUser == NULL)
		return (ERR_USERNOTINCHANNEL(user, target, name), void());

	channel->kickUser(user, targetedUser, reason);
	if (channel->getAllUsers().empty())
		srv->deleteChannel(channel);
}