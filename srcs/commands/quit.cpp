/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:41 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 10:03:22 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: QUIT [<reason>]

void	Command::_quit(std::vector<std::string> args, User *user)
{
	if (user->getConnection() == false)
		return (ERR_NOTREGISTERED(user), void());

	std::string reason = "";
	if ( (args.size() > 1) && (args[1][0] == ':') )
	{
		reason = args[1].substr(1);
		for (size_t i = 2; i < args.size(); i++)
			reason.append(" " + args[i]);
	}

	this->_srv->broadcastIgnore(RPL_QUIT(user->getPrefix(), reason), user);
	std::vector<std::string> joinArgs;
	joinArgs.push_back("JOIN");
	joinArgs.push_back("0");
	this->_join(joinArgs, user);
	_srv->onUserDropped(user->getfd());
}
