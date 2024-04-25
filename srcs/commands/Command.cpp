/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:28:06 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:28:08 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Command.hpp"
# include "Utils.hpp"

Command::Command(Server *_srv) : _srv(_srv)
{
	this->add("PASS",		&Command::_pass);
	this->add("NICK",		&Command::_nick);
	this->add("USER",		&Command::_user);
	this->add("QUIT",		&Command::_quit);
	this->add("PING",		&Command::_ping);
	this->add("JOIN",		&Command::_join);
	this->add("PART",		&Command::_part);
	this->add("KICK",		&Command::_kick);
	this->add("INVITE",		&Command::_invite);
	this->add("TOPIC",		&Command::_topic);
	this->add("PRIVMSG",	&Command::_privmsg);
	this->add("MODE",		&Command::_mode);
}

Command::~Command() {}

void	Command::add(std::string command, CommandFunction f)
{
	_commands.insert(std::pair<std::string, CommandFunction>(command, f));
}


void Command::parse(std::string ClientCmd, int userFd)
{
	std::vector<std::string> arguments;
	std::string currentLine;

	std::cout << "Client("<<userFd<<") sends :" << std::endl;
	std::cout << ClientCmd << std::endl;

	for (size_t i = 0; i < ClientCmd.size(); i++)
	{
		if (ClientCmd[i] == '\n')
		{
			currentLine = ClientCmd.substr(0, i); // Récupère la cmd dans la string
			ClientCmd = ClientCmd.erase(0, i + 1); // Supprime ce qui a été parcouru
			i = 0;
			for (size_t j = 0; j < currentLine.size(); j++) // Parcours la cmd et récupère chaque argument
			{
				if (currentLine[j] == ' ' || j == currentLine.size() - 1)
				{
					arguments.push_back(currentLine.substr(0, j + (j == currentLine.size() - 1)));
					currentLine = currentLine.substr(j + 1);
					if (currentLine.size() == 1)
						arguments.push_back(currentLine);
					j = 0;
				}
			}
			if (arguments.size() > 0)
			{
				this->execute(arguments[0], arguments, userFd);
			}
			arguments.clear();
			currentLine.clear();
		}
	}
}

bool	Command::isExecutable(User *user, std::string msg)
{
	std::string	commands[] = {"PASS", "NICK", "USER", "QUIT"};
	if (user->getConnection() == false)
	{
		for (size_t i = 0; i < commands->size(); i++)
			if (!commands[i].compare(msg))
				return (true);

		ERR_NOTREGISTERED(user);
		return (false);
	}
	return (true);
}

void	Command::execute(std::string command, std::vector<std::string> args, int userFd)
{
	std::map<std::string, CommandFunction>::iterator it = _commands.find(command);
	User *user = _srv->getUserByFd(userFd);
	if (user && it != _commands.end() && this->isExecutable(user, command))
	{
		std::cout << "	EXEC COMMAND :" << command << std::endl;
		(this->*(it->second))(args, user);
		std::cout << "	END OF EXEC COMMAND :" << command << std::endl << std::endl;
	}
}

void	toClient(int fd, const std::string &message)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
    else
	{
	    std::cout << "Send to client("<<fd<<"):" << std::endl;
	    std::cout << cpyWithoutCR(message) << std::endl;
    }
}
