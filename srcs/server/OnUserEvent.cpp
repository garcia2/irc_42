/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OnUserEvent.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:07 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/25 10:41:14 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

const std::map<int, User *> 	&Server::getUsers() const { return (this->_users); }

User	*Server::getUserByFd(int fd) const
{
	std::map<int, User *>::const_iterator it = this->_users.find(fd);
	if (it == this->_users.end())
		return (NULL);
	return (it->second);
}

User	*Server::getUserByNickname(const std::string &nickname) const
{
	for (std::map<int, User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
		if (it->second->getNickname() == nickname)
			return (it->second);

	return (NULL);
}

void	Server::onUserMessage(Command &command, int fd)
{
	char buffer[1024];
	std::string message;

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes < 0)
		return;
	else if (bytes == 0)
	{
		User * user = getUserByFd(fd);
		if (user != NULL && !user->getNickname().empty() && user->getRegistration())
		{
			std::vector<std::string> joinArgs;
			joinArgs.push_back("JOIN");
			joinArgs.push_back("0");
			command._join(joinArgs, user);
			
		}
		onUserDropped(fd);
		return;
	}
	buffer[bytes] = '\0';
	this->buffersMap[fd].push_back(buffer);
	if (buffer[bytes - 1] == '\n')
	{
		message = buffersConcat(this->buffersMap[fd]);
		this->buffersMap[fd].clear();
		command.parse(cpyWithoutCR(message), fd);
	}
	else
	{
		std::cout << "Client("<<fd<<") partially sends :" << std::endl;
		std::cout << buffer << std::endl;
	}
	std::cout << std::endl;
}


void Server::onUserConnecting()
{
    sockaddr_in addr = {};
    socklen_t size = sizeof(addr);

    // Accepter une nouvelle connexion client en utilisant la fonction accept().
    // Crée une nouvelle socket connectée au client et retourne un descripteur de fichier associé à cette socket.
    int fd = accept(this->_socket, (sockaddr *)&addr, &size);
    if (fd < 0)
        return;

    pollfd srv;
    srv.fd = fd;
    srv.events = POLLIN;
    _pollsets.push_back(srv);

    char hostname[NI_MAXHOST];
    // Récupérer les informations sur le client, telles que son adresse IP et son numéro de port,
    int res = getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        return (std::cout << "Error : new connection failed!" << std::endl, void());

    User *user = new User(srv);
    _users.insert(std::pair<int, User *>(fd, user));
}


void	Server::onUserDropped(int userFd)
{
	User * user = this->getUserByFd(userFd);
	if (user != NULL)
	{
		if (!user->getNickname().empty() && user->getRegistration())
			logTime(user->getNickname() + " has disconnected");
		delete (this->_users[userFd]);
		this->_users.erase(userFd);
	}
	this->buffersMap[userFd].clear();
	close(userFd);
}
