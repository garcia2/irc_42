/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:53 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 11:37:05 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: PASS <password>

void Command::_pass(std::vector<std::string> args, User *user)
{
	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "PASS"), void());

	if (user->getConnection() == true)
		return (ERR_ALREADYREGISTERED(user), void());

	std::string strToCompare = args[1].substr(0, args[1].size() - (args[1][args[1].size() - 1] == 13) );

	if (_srv->getPassword().substr().compare(strToCompare) != 0)
		return (ERR_PASSWDMISMATCH(user), void());
	user->setConnection(true);
}
