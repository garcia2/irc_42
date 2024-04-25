/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:27:12 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 11:22:52 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


// syntax: JOIN <channel>{,<channel>} [<key>{,<key>}]

void	_joinProcess(std::string channelName, std::string password, Server * srv, User * user);
void	send_command(User *user, Channel *channel);
bool	isValidChannelName(std::string &str);


void Command::_join(std::vector<std::string> args, User *user)
{
	if (user->getNickname().empty() || user->getRegistration() == false)
		return (ERR_NOTREGISTERED(user), void());

	if (args.size() < 2)
		return (ERR_NEEDMOREPARAMS(user, "JOIN"), void ());

	if (args[1] == "0")
	{
		std::map<std::string, Channel *> channels = this->_srv->getAllChannels();
		std::vector<std::string> channelNames;
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (it->second->getUser(user->getNickname()) != NULL)
				channelNames.push_back(it->second->get_name());
		}
		std::vector<std::string> partArgs;
		partArgs.resize(2);
		partArgs[0] = "PART";
		for (size_t i = 0; i < channelNames.size(); i++)
		{
			partArgs[1] = channelNames[i];
			this->_part(partArgs, user);
		}
		return ;
	}
	
	std::vector<std::string> channelNames = splitString(args[1]);
	std::vector<std::string> passwords;
	if (args.size() >= 3)
		passwords = splitString(args[2]);
	
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		if (i < passwords.size())
			_joinProcess(channelNames[i], passwords[i], this->_srv, user);
		else
			_joinProcess(channelNames[i], "", this->_srv, user);
	}
}

void	_joinProcess(std::string channelName, std::string password, Server * srv, User * user)
{
	if (!isValidChannelName(channelName))
		return (ERR_NOSUCHCHANNEL(user, channelName));

	Channel * channel = srv->getChannel(channelName);

	if (channel == NULL)
	{	
		if (!password.empty())
			channel = srv->addChannelPassowrd(channelName, user, password);
		else
			channel = srv->addChannel(channelName, user);

		std::cout << "Channel created :" << channelName << std::endl;
	}
	else
	{
		if (channel->getUser(user->getNickname()) != NULL)
			return ;
		
		if (channel->get_limit() != 0 && channel->getAllUsers().size() >= channel->get_limit())
			return (ERR_CHANNELISFULL(user, channelName), void());

		if (!channel->get_password().empty() && channel->get_password() != password)
			return (ERRPASSWORDCHANNEL(user, channelName), void());

		if (channel->isInviteOnly())
		{
			if (channel->isUserInvited(user) == false)
				return (ERR_INVITEONLYCHAN(user, channelName), void());
			channel->deleteInvitation(user);
		}

		channel->addUser(user);
	}

	send_command(user, channel);
}

void send_command(User *user, Channel *channel)
{
	channel->broadcast( RPL_JOIN(user->getPrefix(), channel->get_name()) );
	if (channel->getTopic().empty() == false)
		RPL_TOPIC(user, channel->get_name(), channel->get_topic());
	RPL_NAMEREPLY(user, channel->get_name(), channel->format_names());
	RPL_ENDOFNAMES(user, channel->get_name());
}

bool isValidChannelName(std::string &str) {
    // Check if the first character is '#'
    if (str.empty()) {
        return false;
    }
	else if (str[0] != '#')
	{
		str = "#" + str;
	}

    // Check the rest of the string
    for (std::string::size_type i = 1; i < str.size(); ++i) {
        char c = str[i];
        if (c == ' ' || c == '\x07' || c == ',') {
            return false;
        }
    }

    return true;
}