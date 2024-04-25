/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:29:15 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:29:17 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef Command_HPP
# define Command_HPP

# include <climits>
# include "Server.hpp"
# include "Message.hpp"
# include "Utils.hpp"

class Server;
class User;

class Command
{
	private :
		Server *_srv;

		typedef void (Command::*CommandFunction)(std::vector<std::string>, User *);
		std::map<std::string, CommandFunction> _commands;

	public :
		Command(Server *_srv);
		virtual ~Command();

		void	add(std::string command, CommandFunction f);
		void	parse(std::string buffer, int userFd);
		bool	isExecutable(User *user, std::string msg);
		void	execute(std::string command, std::vector<std::string> args, int userFd);

		void	_pass(std::vector<std::string> args, User *user);
		void	_nick(std::vector<std::string> args, User *user);
		void	_user(std::vector<std::string> args, User *user);
		void	_quit(std::vector<std::string> args, User *user);
		void	_ping(std::vector<std::string> args, User *user);
		void	_part(std::vector<std::string> args, User *user);
		void	_kick(std::vector<std::string> args, User *user);
		void	_invite(std::vector<std::string> args, User *user);
		void	_join(std::vector<std::string> args, User *user);
		void	_topic(std::vector<std::string> args, User *user);
		void	_privmsg(std::vector<std::string> args, User *user);
		void	_mode(std::vector<std::string> args, User *user);
};

void	toClient(int fd, const std::string &message);

#endif
