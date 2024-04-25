/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:12:47 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/25 10:39:26 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) :
	_name(name),
	_admin(NULL),
	_topic(""),
	_l(0),
	_i(false),
	_t(false),
	_creationTimeChannel(std::time(0))
{	

}


Channel::Channel(const std::string name, const std::string password) :
	_name(name),
	_password(password),
	_topic(""),
	_l(false),
	_i(false),
	_t(false),
	_creationTimeChannel(std::time(0))
{
	
}


Channel::~Channel() {}


std::string Channel::get_password()	const { return (this->_password);}
std::string Channel::get_topic()	const { return (this->_topic); }

std::string Channel::format_names(){
	std::string names;
	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		names += ( (this->isUserOperator(*it) ? "@" : "") + (*it)->getNickname() + " ");
	}
	if (!names.empty())
	{
		names.erase(names.length() - 1);
	}
	return names;	
}

bool	Channel::get_topic_exist() const
{
	return _t;
}

std::string	Channel::get_name()				const	{ return (this->_name); }
User *		Channel::get_admin()			const	{ return (this->_admin); }
std::string	Channel::get_key()				const	{ return (this->_password); }
size_t		Channel::get_limit()			const	{ return (this->_l); }
size_t		Channel::get_size()				const	{ return (this->_users.size()); }
std::string	Channel::getTopic() 			const	{ return (this->_topic); }
bool		Channel::isInviteOnly() 		const	{ return (this->_i); }
bool		Channel::hasTopicRestriction()	const	{ return (this->_t); }


void	Channel::broadcast(const std::string& message)
{	
	std::vector<User *>::iterator begin = this->_users.begin();
	std::vector<User *>::iterator end = this->_users.end();

	while (begin != end)
	{
		(*begin)->write(message);
		begin++;
	}
}

void	Channel::broadcastIgnore(const std::string& message, User * userToIgnore)
{
	std::vector<User *>::iterator begin = this->_users.begin();
	std::vector<User *>::iterator end = this->_users.end();

	while (begin != end)
	{
		if ( (*begin)->getNickname() != userToIgnore->getNickname() )
			(*begin)->write(message);
		begin++;
	}
}

void	Channel::deleteUser(User * user)
{
	std::vector<User *>::iterator begin = this->_users.begin();
	std::vector<User *>::iterator end = this->_users.end();
	User * target;

	while (begin != end)
	{
		target = *begin;
		if (target->getNickname() == user->getNickname()) 
		{
			if (this->isUserOperator(*begin))
				this->deleteOperator(*begin);
			this->_users.erase(begin);
			return;
		}
		begin++;
	}
}

void	Channel::deleteOperator(User * user)
{
	std::vector<User *>::iterator begin = this->_operators.begin();
	std::vector<User *>::iterator end = this->_operators.end();
	User * target;

	while (begin != end)
	{
		target = *begin;
		if (target->getNickname() == user->getNickname()) 
		{
			this->_operators.erase(begin);
			return;
		}
		begin++;
	}
}

void	Channel::deleteInvitation(User * user)
{
	std::vector<User *>::iterator begin = this->_invitedUsers.begin();
	std::vector<User *>::iterator end = this->_invitedUsers.end();
	User * target;

	while (begin != end)
	{
		target = *begin;
		if (target->getNickname() == user->getNickname()) 
		{
			this->_invitedUsers.erase(begin);
			return;
		}
		begin++;
	}
}

void Channel::addUser(User *user) {
    _users.push_back(user);
}

void Channel::addSuperUser(User *user) {
    _operators.push_back(user);
}

User* Channel::getUser(std::string name)
{
	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getNickname() == name)
			return (*it);
	}
	return NULL;
}

void	Channel::kickUser(User * user, User * target, std::string reason)
{
	this->broadcast(RPL_KICK(user->getPrefix(), _name, target->getNickname(), reason));
	this->deleteUser(target);
}

std::string     Channel::getCreationTime() const {
        std::stringstream ss;
		ss << _creationTime;
		std::string s = ss.str();
		return (s);
    }

std::string Channel::get_creationTimeChannel() const 
{
	std::stringstream ss;
	ss << _creationTimeChannel;
	std::string s = ss.str();
	return (s);
}

void			Channel::set_creationTime(std::time_t)
{
	_creationTime = std::time(0);
}

void	Channel::setTopic(User * user, std::string topic)
{
	this->set_creationTime(std::time(0));
	this->_topic = topic;
	this->broadcast(RPL_TOPIC2(user->getPrefix(), this->_name, topic));
}

void	Channel::inviteUser(User * user, User * target)
{
	this->_invitedUsers.push_back(target);
	target->write(RPL_INVITE(user->getPrefix(), target->getNickname(), this->_name));
	user->write(RPL_INVITING(user->getNickname(), target->getNickname(), this->_name));
}


std::vector<User*> Channel::getAllUsers() const {
	return _users;
}

bool	Channel::isUserOperator(User * user)
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNickname() == user->getNickname())
			return (true);
	}
	return (false);
}

bool	Channel::isUserInvited(User * user)
{
	for (size_t i = 0; i < this->_invitedUsers.size(); i++)
	{
		if (this->_invitedUsers[i]->getNickname() == user->getNickname())
			return (true);
	}
	return (false);
}

std::string Channel::getModeStr() const
{
	std::string modeStr = "+";

	if (this->_l != 0)
		modeStr.append("l");
	
	if (this->_i == true)
		modeStr.append("i");

	if (this->_t == true)
		modeStr.append("t");
	
	if (this->_password.empty() == false)
		modeStr.append("k");
	
	if (modeStr.size() == 1)
		modeStr = "";
	
	return (modeStr);
}

void	Channel::setModes(Server *srv, User * user, std::string mode, std::vector<std::string> modeArgs)
{
	
	if (isValidMode(user, mode) == false)
		return ;
	
	bool	addingMode = true;
	std::string execMinusMode = "";
	std::string execPlusMode = "";

	size_t	k = 0;

	for (size_t i = 0; i < mode.size(); i++)
	{
		switch (mode[i])
		{
			case 'l':
				if (addingMode && k >= modeArgs.size())
					ERR_NEEDMOREPARAMS(user, "MODE");
				else
				{
					this->setLimit(addingMode ? modeArgs[k++] : "0");
					if (addingMode)
						execPlusMode.append("l");
					else
						execMinusMode.append("l");
				}
				break;

			case 'i':
				this->_i = addingMode;
				if (addingMode)
						execPlusMode.append("i");
					else
						execMinusMode.append("i");
				break;

			case 't':
				this->_t = addingMode;
				if (addingMode)
						execPlusMode.append("t");
					else
						execMinusMode.append("t");
				break;

			case 'k':
				if (addingMode && k >= modeArgs.size())
					ERR_NEEDMOREPARAMS(user, "MODE");
				else
				{
					this->_password = addingMode ? modeArgs[k++] : "";
					if (addingMode)
						execPlusMode.append("k");
					else
						execMinusMode.append("k");
				}
				break;

			case 'o':
				if (k == modeArgs.size())
					ERR_NEEDMOREPARAMS(user, "MODE");
				else
				{
					setOperator(srv, user, modeArgs[k++], addingMode);
					if (addingMode)
						execPlusMode.append("o");
					else
						execMinusMode.append("o");
				}
				break;

			case '+':
				addingMode = true;
				break;

			case '-':
				addingMode = false;
				break;
    	}
	}
	
	std::string modeRPL = "";
	if (!execPlusMode.empty())
		modeRPL.append("+" + execPlusMode);
	if (!execMinusMode.empty())
		modeRPL.append("-" + execMinusMode);
	this->broadcast(RPL_CHANNEL_MODE(user->getPrefix(), this->get_name(), modeRPL));
}

bool	Channel::isValidMode(User * user, std::string mode)
{
	std::string validMode = "+-litko";
	
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (validMode.find(mode[i]) == std::string::npos)
		{
			std::string errorMode = "";
			errorMode.push_back(mode[i]);
			return (ERR_UNKNOWNMODE(user, errorMode), false);
		}
	}
	return (true);
}

void	Channel::setLimit(std::string limitStr)
{
	char* endPtr;
    long int converted = strtol(limitStr.c_str(), &endPtr, 10);

    if (*endPtr == '\0')
	{
        if (converted >= INT_MIN && converted <= INT_MAX)
            this->_l = static_cast<int>(converted);
    }
}

void	Channel::setOperator(Server * srv, User * user, std::string target, bool addingMode)
{
	User * targetedUser = srv->getUserByNickname(target);
	if (targetedUser == NULL)
		return (ERR_NOSUCHNICK(user, target), void());

	if (this->getUser(target) == NULL)
		return (ERR_USERNOTINCHANNEL(user, target, this->get_name()));

	if ( (addingMode) && (!this->isUserOperator(targetedUser)) )
		this->addSuperUser(targetedUser);
	else if (this->isUserOperator(targetedUser))
		this->deleteOperator(targetedUser);
}