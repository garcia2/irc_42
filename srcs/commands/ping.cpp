/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:46 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 11:25:14 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: PING <token>

void	Command::_ping(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	std::string pong;

	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "PING"), void());

	std::string token = args[1];
	if (token.empty())
		return (ERR_NOORIGIN(user));

	user->write(RPL_PING(token));
}
