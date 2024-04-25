/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:37:14 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:24:04 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: MODE <target> [<modestring> [<mode arguments>...]]

void	Command::_mode(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "MODE"));


	std::string target = args[1];
	if (target[0] == '#')
	{
		// CHANNEL MODE
		
		Channel * channel = this->_srv->getChannel(target);
		if (channel == NULL)
			return (ERR_NOSUCHCHANNEL(user, target), void());

		if (channel->getUser(user->getNickname()) == NULL)
			return (ERR_NOTONCHANNEL(user, target), void());
		
		// if <modestring> is not given
		if (args.size() < 3)
		{
			user->write(RPL_CHANNELMODEIS(user->getNickname(), target, channel->getModeStr()));
			user->write(RPL_CREATIONTIME(user->getNickname(), target, channel->get_creationTimeChannel()));
			return ;
		}
		std::string mode = args[2];	
			
		if (channel->isUserOperator(user) == false)
			return (ERR_CHANOPRIVSNEEDED(user, target));
		
		std::vector<std::string> modeArgs;
		for (size_t i = 3; i < args.size(); i++)
			modeArgs.push_back(args[i]);
		
		channel->setModes(this->_srv, user, mode, modeArgs);
	}
	else
	{
		// USER MODE
		User * targetedUser = this->_srv->getUserByNickname(target);

		if (targetedUser == NULL)
			return (ERR_NOSUCHNICK(user, target), void());

		if (target != user->getNickname())
			return (ERR_USERSDONTMACTCH(user), void());

		if (args.size() < 3)
		{
			user->write(RPL_UMODEIS(user->getNickname()));
			return ;
		}

		std::string mode = args[2];
		user->write(RPL_CHANNEL_MODE(user->getPrefix(), user->getNickname(), mode));
	}

}