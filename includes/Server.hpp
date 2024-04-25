/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:28:31 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:28:33 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <csignal>
# include <sstream>
# include <string>

# include <csignal>
# include <sys/socket.h>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <poll.h>
# include <fcntl.h>
# include <unistd.h>

# include <vector>
# include <map>
# include <stdexcept>
# include <cerrno>

# include "User.hpp"
# include "Command.hpp"
# include "Message.hpp"
# include "Channel.hpp"

class User;
class Command;
class Channel;

# define to_string(value) static_cast<const std::ostringstream&>((\
std::ostringstream() << std::dec << value)).str()

# define MAX_CONNECTIONS 999

# define MUTE_PING true

class 	Server
{
	private :
		std::string				_password;
		std::string				_host;
		int						_port;

		int						_socket;

		std::vector<pollfd>     _pollsets;
		std::map<int, User *>	_users;
		std::map<std::string, Channel *> _channels;

		std::map< int, std::vector<std::string> > buffersMap;
		Server(Server const &cpy);
		Server &operator=(Server const &cpy);

	public :
		Server();
		Server(int port, std::string password);
		~Server();


		bool								checkIfExist(std::string user);
		bool								checkifChanExist(std::string channelName);
		const int							&getPort()										const;
		const std::string					&getHost()										const;
		const std::string					&getPassword()									const;
		const std::map<int, User *> 		&getUsers()										const;
		User								*getUserByFd(int fd)							const;
		User								*getUserByNickname(const std::string &nickname)	const;
		std::map<std::string, Channel *>	getAllChannels();


		void				DisconnectUser();

		static				void signalHandler(int signal);
		int					initializeSocket();
		void				run();
		void				logTime(const std::string &message);

		void				onUserConnecting();
		void				onUserMessage(Command &command, int fd);
		void				onUserDropped(int	userFd);

		void				sendMessageToChannel(const std::string& channelName, const std::string& message);
		void				sendMessageToUser(const std::string& userName, const std::string& message);

		Channel *			getChannel(std::string name);
		Channel *			addChannel(std::string Name, User *user);
		Channel *			addChannelPassowrd(std::string name, User *user, std::string password);
		Channel* 			findChannelByName(const std::string& name);
		void				listChannels(void);
		void				deleteChannel(Channel * channel);
		void				broadcast(const std::string& message);
		void				broadcastIgnore(const std::string& message, User * userToIgnore);
};
#endif
