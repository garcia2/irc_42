/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:45:02 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/23 14:19:20 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: INVITE <nickname> <channel>

void	Command::_invite(std::vector<std::string> args, User *user)
{
	if (user->getConnection() == false || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 3)
		return (ERR_NEEDMOREPARAMS(user, "INVITE"), void());

	std::string targetName = args[1];
	std::string channelName = args[2];

	Channel * channel = this->_srv->getChannel(channelName);
	if (channel == NULL)
		return (ERR_NOSUCHCHANNEL(user, channelName), void());

	if (channel->getUser(user->getNickname())  == NULL)
		return (ERR_NOTONCHANNEL(user, channelName), void());

	User * target = this->_srv->getUserByNickname(targetName);
    if (target == NULL)
		return (ERR_NOSUCHNICK(user, targetName), void());

	if ( (channel->isInviteOnly()) && (channel->isUserOperator(user) == false) )
		return (ERR_CHANOPRIVSNEEDED(user, channelName), void());

	if (channel->getUser(target->getNickname())  != NULL)
		return (ERR_USERONCHANNEL(user, channelName, targetName), void());

	channel->inviteUser(user, target);
}