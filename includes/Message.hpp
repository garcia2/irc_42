/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:29:09 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 10:31:14 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <vector>
# include <iostream>

# define SEND toClient
# define RN + "\r\n"

// ERROR
# define ERR_ALREADYREGISTERED(user) \
SEND (user->getfd(), ":localhost 462 " + (user->getNickname().empty() ? "*" : user->getNickname()) + " :Unauthorized command (already registered)" RN)

# define ERR_NOTREGISTERED(user) \
SEND (user->getfd(), ":localhost 451 " + (user->getNickname().empty() ? "*" : user->getNickname()) + " :You have not registered" RN)

# define ERR_NONICKNAMEGIVEN(user) \
SEND (user->getfd(), std::string(":localhost 431 :No nickname given") RN)

# define ERR_NICKNAMEINUSE(user, nickname) \
SEND (user->getfd(), ":localhost 433 " + (user->getNickname().empty() ? "*" : user->getNickname()) + " " + nickname + " :Nickname is already in use" RN)

# define ERR_ERRONEUSNICKNAME(user, nickname) \
SEND (user->getfd(), ":localhost 432 " + (user->getNickname().empty() ? "*" : user->getNickname()) + " :Erroneous nickname" RN)

# define ERR_NEEDMOREPARAMS(user, command) \
SEND (user->getfd(), ":localhost 461 " + (user->getNickname().empty() ? "*" : user->getNickname()) + " " + command + " :Not enough parameters" RN)

# define ERR_PASSWDMISMATCH(user)	\
SEND (user->getfd(), ":localhost 464 " + user->getNickname() + " :Password incorrect" RN)

#define ERRPASSWORDCHANNEL(user, channel) \
SEND (user->getfd(), ":localhost 475 " + user->getNickname() + " " + channel + " :Cannot join channel (+k) - bad key" RN)

#define ERR_NOTONCHANNEL(user, channel)	\
SEND (user->getfd(), ":localhost 442 " + user->getNickname() + " " + channel + " :You're not on that channel" RN)

#define ERR_NOSUCHCHANNEL(user, channel)	\
SEND (user->getfd(), ":localhost 403 " + user->getNickname() + " " + channel + " :No such channel" RN)

#define ERR_CHANOPRIVSNEEDED(user, channel)	\
SEND (user->getfd(), ":localhost 482 " + user->getNickname() + " " + channel + " :You're not channel operator" RN)

#define ERR_USERNOTINCHANNEL(user, target ,channel)	\
SEND (user->getfd(), ":localhost 441 " + user->getNickname() + " " + target + " " + channel + " :They aren't on that channel" RN)

#define ERR_NOSUCHNICK(user, nickname)	\
SEND (user->getfd(), ":localhost 401 " + user->getNickname() + " " + nickname + " :No such nick/channel" RN)

#define ERR_USERONCHANNEL(user, nickname, channel)	\
SEND (user->getfd(), ":localhost 443 " + user->getNickname() + " " + nickname + " " + channel + " :is already on channel" RN)

#define ERR_NORECIPIENT(user, command) \
SEND (user->getfd(), ":localhost 411 " + user->getNickname() + " " + command + " :No recipient given" RN)

#define ERR_NOTEXTTOSEND(user) \
SEND (user->getfd(), ":localhost 412 " + user->getNickname() + " :No text to send" RN)

#define ERR_TOOMANYTARGETS(user, target) \
SEND (user->getfd(), ":localhost 407 " + user->getNickname() + " " + target + " :Too many targets. Maximum is 1." RN)

# define ERR_CANNOTSENDTOCHAN(user, channel) \
SEND (user->getfd(), ":localhost 404 " + user->getNickname() + " " + channel + " :Cannot send to channel" RN)

#define ERR_USERSDONTMACTCH(user)	\
SEND (user->getfd(), ":localhost 502 " + user->getNickname() + " :Cant change mode for other users" RN)

#define ERR_NOORIGIN(user)	\
SEND (user->getfd(), ":localhost 409 " + user->getNickname() + " :No origin specified" RN)

#define ERR_UNKNOWNMODE(user, mode)	\
SEND (user->getfd(), ":localhost 472 " + user->getNickname() + " " + mode + " :is unknown mode to me" RN)

#define ERR_INVITEONLYCHAN(user, channel)	\
SEND (user->getfd(), ":localhost 473 " + user->getNickname() + " " + channel + " :Cannot join channel (+i)" RN)

#define ERR_CHANNELISFULL(user, channel) \
SEND (user->getfd(), ":localhost 471 " + user->getNickname() + " " + channel + " :Cannot join channel (+l)" RN)


// REPLY CMD
#define RPL_TOPIC(user, channel, topic) \
SEND (user->getfd(), ":localhost 332 " + user->getNickname() + " " + channel + " :" + topic RN)

#define RPL_NAMEREPLY(user, channel, names) \
SEND (user->getfd(), ":localhost 353 " + user->getNickname() + " = " + channel + " :" + names RN)

#define RPL_ENDOFNAMES(user, channel) \
SEND (user->getfd(), ":localhost 366 " + user->getNickname() + " " + channel + " :End of /NAMES list." RN)

// #define RPL_TOPICWHOTIME(user, channel, topicSetter, time) SEND (user->getfd(), ":localhost 333 " + user->getNickname() + " " + channel + " " + topicSetter + " " + time + RN)

#define RPL_WELCOME(nickname) \
std::string(":localhost 001 " + nickname + " :Welcome to the Internet Relay Network, " + nickname)

#define RPL_YOURHOST(nickname) \
std::string(":localhost 002 " + nickname + " :Your host is localhost, running version CPP.9.8")

#define RPL_CREATED(nickname) \
std::string(":localhost 003 " + nickname + " :This server was created lundi 01 avril (UTC+2) at 2024, 09:08:01")

#define RPL_MYINFO(nickname) \
std::string(":localhost 004 " + nickname + " localhost CPP.9.8 litko lko")

#define RPL_ISUPPORT(nickname) \
std::string(":localhost 005 " + nickname + " # :are supported by this server")

// #define RPL_TOPICWHOTIME(nickname, channel, topicSetter, time) std::string(":localhost 333 " + nickname + " " + channel + " " + topicSetter + " " + time)

#define RPL_NICK(prefix, newNickname) \
std::string(":" + prefix + " NICK :" + newNickname)

#define RPL_CREATIONTIME_WITHOUT_TO_CLIENT(nickname, channel, times) \
std::string(":localhost 333 " + nickname + " " + channel + " " + times + " :channel creation time\r\n")

#define RPL_PART(prefix, channel, reason) \
std::string(":" + prefix + " PART " + channel + (reason.empty() ? "" : "  :") + reason)

#define RPL_KICK(prefix, channel, target, reason) \
std::string(":" + prefix + " KICK " + channel + " " + target + " :" + reason)

#define RPL_NOTOPIC(nickname, channel) \
std::string(": 331" + nickname + " " + channel + " :No topic is set")

#define RPL_TOPIC2(prefix, channel, topic) \
std::string(":" + prefix + " TOPIC " + channel + " :" + topic)

#define RPL_INVITING(nickname, target, channel) \
std::string(": 341 " + nickname + " " + target + " " + channel)

#define RPL_INVITE(prefix, target, channel) \
std::string(":" + prefix + " INVITE " + target + " " + channel)

#define RPL_JOIN(prefix, channel) \
std::string (":" + prefix + " JOIN " +  channel)

#define RPL_PING(token) \
std::string(":localhost PONG " + token)

#define RPL_CHANNELMODEIS(nickname, channel, channelMode) \
std::string(":localhost 324 " + nickname + " " + channel + " " + channelMode)

#define RPL_UMODEIS(nickname) \
std::string(":localhost 221 " + nickname + " " + nickname)

#define RPL_CHANNEL_MODE(prefix, channel, channelMode) \
std::string(":" + prefix + " MODE " + channel + " " + channelMode)

#define RPL_PRIVMSG(prefix, channel, msg) \
std::string(":" + prefix + " PRIVMSG " + channel + " :" + msg)

#define RPL_QUIT(prefix, reason) \
std::string(":" + prefix + " QUIT :Quit: " + reason)

#define RPL_CREATIONTIME(nickname, channel, time) \
std::string(":localhost 329 " + nickname + " " + channel + " " + time )


#endif
