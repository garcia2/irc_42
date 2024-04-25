/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:58 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:28:00 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// syntax: NICK <nickname>

void	Command::_nick(std::vector<std::string> args, User *user)
{
	if (user->getConnection() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 2)
		return (ERR_NONICKNAMEGIVEN(user), void());

	if (args.size() != 2)
		return (ERR_NEEDMOREPARAMS(user, "NICK"), void());

	std::string nickName = cpyWithoutCR(args[1]);

	std::string forbiddenChars = "# :[]\\`^{|}";
	if (nickName.find_first_of(forbiddenChars) != std::string::npos || !isValidNickName(nickName))
		return (ERR_ERRONEUSNICKNAME(user, nickName), void());

	std::map<int, User *> users = _srv->getUsers();
	for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->second->getfd() != user->getfd() && it->second->getNickname() == nickName)
			return (ERR_NICKNAMEINUSE(user, nickName), void());
	}

	user->write(RPL_NICK(user->getPrefix(), nickName));
	user->setNickname(nickName);
	if (user->getRegistration() == false)
		return ;
	user->write(RPL_WELCOME(user->getNickname()));
	user->write(RPL_YOURHOST(user->getNickname()));
	user->write(RPL_CREATED(user->getNickname()));
	user->write(RPL_MYINFO(user->getNickname()));
	user->write(RPL_ISUPPORT(user->getNickname()));
}
