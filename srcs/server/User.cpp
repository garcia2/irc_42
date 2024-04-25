/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:01 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 10:41:34 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include "Utils.hpp"

User::User() {}

User::User(pollfd newPoll) : _poll(newPoll)
{
	this->_nickname = "";
	this->_username = "";
	this->_realName = "";
	this->_isConnected = false;
	this->_isRegistrated = false;
	// this->_channel = NULL;
}

User::User(User const &cpy)
{
	*this = cpy;
}

User &User::operator=(User const &cpy)
{
	this->_poll = cpy._poll;
	this->_nickname = cpy._nickname;
	this->_username = cpy._username;
	return (*this);
}

User::~User(void) {}

const std::string	&User::getNickname(void) const { return (this->_nickname); }
const std::string	&User::getUsername(void) const { return (this->_username); }
const std::string	&User::getRealName(void) const { return (this->_realName); }
const pollfd		&User::getPoll(void) const { return (this->_poll); }
const int			&User::getfd(void) const { return (this->_poll.fd); }
const bool			&User::getConnection(void) const { return (this->_isConnected); }
bool				User::getRegistration(void) const { return (this->_isRegistrated); };

void	User::setNickname(const std::string &nickname) { this->_nickname = nickname; }
void	User::setUsername(const std::string &username) { this->_username = username; }
void	User::setRealName(const std::string &realName) { this->_realName = realName; }
void	User::setConnection(const bool &connection) { this->_isConnected = connection; }
void	User::setRegistration(bool registration) { this->_isRegistrated = registration; };


void	User::write(const std::string& message) const
{
	std::cout << "Send to client("<<this->getfd()<<"):" << std::endl;
	std::cout << cpyWithoutCR(message) << std::endl;
	std::string buffer = message + "\r\n";
	if (send(this->getfd(), buffer.c_str(), buffer.length(), 0) < 0)
		std::cerr << "Send to client("<<this->getfd()<<") failed" << std::endl;
}

void	User::leaveChannel(std::string reason, Channel * channel)
{
	if (channel->getUser(this->getNickname()) == NULL)
		return ;

	std::string message = RPL_PART(this->getPrefix(), channel->get_name(), reason);
	channel->broadcast(message);
	channel->deleteUser(this);
}

std::string	User::getPrefix(void) const
{
	return ( (this->getNickname().empty() ? "*" : this->getNickname()) + "!" + this->getUsername() + "@localhost");
}
