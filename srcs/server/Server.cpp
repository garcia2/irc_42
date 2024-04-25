/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:26:55 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 15:25:10 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool _executing = true;

Server::Server() {}

Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_host = "localhost";
	this->_socket = this->initializeSocket();
}

Server::~Server()
{
	std::map<int, User *>::iterator it2 = this->_users.begin();
	while (it2 != this->_users.end())
	{
		delete (it2->second);
		close(it2->first);
		it2++;
	}
	this->_users.clear();
	std::map<std::string, Channel *>::iterator it = this->_channels.begin();
	while (it != this->_channels.end())
	{
		delete (it->second);
		it++;
	}
	this->_channels.clear();
}

const int						&Server::getPort() const { return (this->_port); }
const std::string				&Server::getHost() const { return (this->_host); }
const std::string				&Server::getPassword() const { return (this->_password); }

void	Server::signalHandler(int signal)
{
	if (signal == SIGINT) // Stop le server en cas de Ctrl + C.
			_executing = false;
}

int	Server::initializeSocket()
{
	int optVal = 1;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // Socket = Point de communication (fd), AF_INET = Domaine d'adresse IP (IPv4).
	// SOCKSTREAM = Spécifie le type de socket (Socket de flux TCP), 0 = protocole de communication par défaut (flux TCP).
	if (sock_fd < 0)
		throw std::runtime_error("Error : socket() failed!");

	// Défini les options de socket pour réutiliser l'IP et le port même si la socket n'est pas close() permet d'accepter plusieurs connnexions.
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)))
		throw std::runtime_error("Error : setsockopt() failed!");

	// Défini les opérations de lecture et d'écriture en mode non bloquant.
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("Error : fcntl() failed!");

	struct sockaddr_in  service; // Représente l'adresse IPv4 de la socket.
	service.sin_family = AF_INET; // Famille de l'adresse.
	service.sin_addr.s_addr = INADDR_ANY; // Acceptes les connexions via toutes les adresses IP de la machine.
	service.sin_port = htons(this->_port); // Défini le port d'écoute.

	if (bind(sock_fd, (sockaddr *)&service, sizeof(service)) < 0) {// Permet de lié la socket à l'adresse IP.
		close (sock_fd);
		throw std::runtime_error("Error : bind() failed!");
	}

	if (listen(sock_fd, MAX_CONNECTIONS) < 0) // Permet à la socket d'accepter les connexions entrantes.
		throw std::runtime_error("Error : listen() failed!");
	return (sock_fd);
}

void	Server::logTime(const std::string &message)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	if (ltm->tm_hour < 10)
		std::cout << "[0" << ltm->tm_hour << ":";
	else
		std::cout << "[" << ltm->tm_hour << ":";
	if (ltm->tm_min < 10)
		std::cout << "0" << ltm->tm_min << "] ";
	else
		std::cout << ltm->tm_min << "]";
	std::cout << " -> " << message << std::endl;
}

void	Server::run()
{
	pollfd srv; // Structure utilisée pour spécifier un ensemble de fd à surveiller par l'appel système poll().
	// Elle contient des informations sur un fd à surveiller et les événements à surveiller pour ce descripteur.
	srv.fd = this->_socket;
	srv.events = POLLIN; // Indique à la fonction poll() de surveiller les événements d'entrée (lecture) sur le fd.
	_pollsets.push_back(srv);

	Command command(this);
	logTime("Server is running on port " + to_string(this->_port));
	while (_executing)
	{
		if (poll(_pollsets.data(), _pollsets.size(), -1) < 0) // Surveille les évènements.
			break;

		for (std::vector<pollfd>::iterator it = _pollsets.begin(); it != _pollsets.end(); it++)
		{
			if (it->revents == 0) // si pas d'event on continue d'ecouter
				continue;
			if ((it->revents & POLLIN) == POLLIN && it->fd == this->_socket) // POLLIN = evenement d'entree
			{
				this->onUserConnecting();
				break;
			}
			this->onUserMessage(command, it->fd);
		}
	}
	this->_pollsets.clear();
	close(this->_socket);
}

Channel *	Server::getChannel(std::string name)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
	if (it == this->_channels.end())
		return (NULL);

	return (it->second);
}

Channel* Server::findChannelByName(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        return it->second; // Return the Channel pointer if found
    }
    return NULL; // Return NULL if not found
}

Channel * Server::addChannel(std::string name, User* user)
{
	Channel* channel = new Channel(name); // Create a new channel object
	channel->addUser(user); // Add the user to the channel
	channel->addSuperUser(user); //add this boy to the super mega list of operator since he s the first one ets gog
	this->_channels[name] = channel; // Add the channel to the map of channels
	if (channel == NULL)
		std::cout << " channel not created" << std::endl;

	return channel;
}

Channel *	Server::addChannelPassowrd(std::string name, User *user, std::string password)
{
	Channel* channel = new Channel(name, password);
	channel->addUser(user);
	channel->addSuperUser(user);
	this->_channels[name] = channel;

	return (channel);
}

bool Server::checkIfExist(std::string user) {
    for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
        User* currentUser = it->second;
        if (currentUser->getNickname() == user) {
            return true;
        }
    }
    return false;
}

bool Server::checkifChanExist(std::string channelName) {
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->first == channelName) {
			return true;
		}
    }
    return false;
}


void Server::deleteChannel(Channel *channel)
{
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_name() == channel->get_name())
		{
			std::cout << "Channel deleted :" << channel->get_name() << std::endl;
			_channels.erase(it);
			delete channel;
			return;
		}
	}
}

std::map<std::string, Channel *>	Server::getAllChannels()
{
	return (this->_channels);
}

void	Server::broadcast(const std::string& message)
{	
	for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		it->second->write(message);
	}
}

void	Server::broadcastIgnore(const std::string& message, User * userToIgnore)
{
	for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (it->second->getNickname() != userToIgnore->getNickname())
			it->second->write(message);
	}
}
