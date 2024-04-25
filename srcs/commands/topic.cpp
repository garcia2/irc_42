/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:04:25 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:25:36 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: TOPIC <channel> [<topic>]

void	Command::_topic(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "TOPIC"), void());

	std::string channelName = args[1];
	Channel * channel = this->_srv->getChannel(channelName);

	if (channel == NULL)
		return (ERR_NOSUCHCHANNEL(user, channelName), void());

	if (channel->getUser(user->getNickname()) == NULL)
		return (ERR_NOTONCHANNEL(user, channelName), void());

	if (args.size() == 2 || args[2][0] != ':')
	{
		if (channel->getTopic().empty())
		{
			user->write(RPL_NOTOPIC(user->getNickname(), channelName));
		}
		else
		{
			user->write(RPL_TOPIC2(user->getPrefix(), channelName, channel->getTopic()));
		}
	}
	else
	{
		if ( (channel->hasTopicRestriction()) && (channel->isUserOperator(user) == false) )
			return (ERR_CHANOPRIVSNEEDED(user, channelName), void());

		std::string topic = args[2].substr(1);
		for (size_t i = 3; i < args.size(); i++)
			topic.append(" " + args[i]);
		
		channel->setTopic(user, topic);
	}
}