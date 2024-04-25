/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:27 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 11:22:22 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: USER <username> 0 * <realneame>

 void    Command::_user(std::vector<std::string> args, User *user)
{
	if (user->getConnection() == false)
		return (ERR_NOTREGISTERED(user), void());

	if ( (args.size() < 5) || (args[1].empty()) )
		return (ERR_NEEDMOREPARAMS(user, "USER"), void());

	std::string username = args[1];

	std::string realname = args[4].substr(1);
	for (size_t i = 5; i < args.size(); i++)
		realname.append(" " + args[i]);


	user->setUsername(args[1]);
	user->setRealName(args[4]);
	user->setRegistration(true);

	if (user->getNickname().empty())
		return ;

	if (user->getUsername().empty())
		_srv->logTime(user->getNickname() + " joined the server");
	user->write(RPL_WELCOME(user->getNickname()));
	user->write(RPL_YOURHOST(user->getNickname()));
	user->write(RPL_CREATED(user->getNickname()));
	user->write(RPL_MYINFO(user->getNickname()));
	user->write(RPL_ISUPPORT(user->getNickname()));
}
