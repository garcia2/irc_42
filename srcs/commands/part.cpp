/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:04:25 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:25:06 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: PART <channel>{,<channel>} [<reason>]

void	_partProcess(std::string name, std::string reason, User * user, Server * srv);


void	Command::_part(std::vector<std::string> args, User *user)
{	
	if (user->getNickname().empty()|| user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "PART"), void());

	std::string reason = "";
	if ( (args.size() > 2) && (args[2][0] == ':') )
	{
		reason = args[2].substr(1);
		for (size_t i = 3; i < args.size(); i++)
			reason.append(" " + args[i]);
	}

	std::vector<std::string> names = splitString(args[1]);
	
	for (size_t i = 0; i < names.size(); i++)
		_partProcess(names[i], reason, user, this->_srv);
}

void	_partProcess(std::string name, std::string reason, User * user, Server * srv)
{
	Channel * channel = srv->getChannel(name);
	if (channel == NULL)
		return (ERR_NOSUCHCHANNEL(user, name), void());

	if ( channel->getUser(user->getNickname()) == NULL )
		return (ERR_NOTONCHANNEL(user, name), void());

	user->leaveChannel(reason, channel);
	if (channel->getAllUsers().empty())
		srv->deleteChannel(channel);
}
