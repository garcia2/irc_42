/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:29:00 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:29:02 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

# include "Channel.hpp"
# include "Server.hpp"
# include <map>

class User
{
    private :
        std::string	_nickname;
        std::string	_username;
        std::string	_realName;
        bool		_isConnected;
		bool		_isRegistrated;
        pollfd		_poll;
		

    public :
        User();
		User &operator=(User const &cpy);
        User(pollfd newPoll);
        User(User const &cpy);
        ~User();


        const std::string	&getNickname(void) const;
        const std::string	&getUsername(void) const;
        const std::string	&getRealName(void) const;
        const pollfd		&getPoll(void) const;
		const int			&getfd(void) const;
		const bool			&getConnection(void) const;
		bool				getRegistration(void) const;
        std::string         getPrefix(void) const;

        void    setNickname(const std::string &nickname);
        void    setUsername(const std::string &username);
        void    setRealName(const std::string &realName);
        void    setConnection(const bool &connection);
		void	setRegistration(bool registration);

        void	write(const std::string& message) const;

		void	leaveChannel(std::string reason);
		void	sendMessage(const std::string& message);
		void	leaveChannel(std::string reason, Channel * channel);
};

#endif
