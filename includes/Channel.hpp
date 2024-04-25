/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nklingsh <nklingsh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:58:16 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/24 13:03:28 by nklingsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include "User.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include <ctime>

class User;
class Server;

class Channel 
{
	typedef std::vector<User *>::iterator client_iterator;

	private:

		std::string				_name;
		std::string				_password;
		
		User *					_admin;
		std::vector<User *>		_users;
		std::vector<User *>		_operators;
		std::vector<User *>		_invitedUsers;
		std::time_t 			_creationTime;
		std::string				_topic;

		/* Modes */
		size_t                  _l; // limit of channel members
		bool					_i;	// yes/no invite only
		bool					_t;	// yes/no topic restriction
		std::time_t 			_creationTimeChannel;

		Channel();
		Channel(const Channel& src);
    
    public:

		/* Constructor and Destructor */

		Channel(std::string name);
		Channel(const std::string name, const std::string password);
		~Channel();


		/* Getters */

		std::string					get_name() const;
		User *						get_admin() const;
		std::string					get_key() const;
		size_t						get_limit() const;
		size_t						get_size() const;
		std::string					get_topic() const;
		std::string					get_creationTimeChannel() const ;
		

		std::vector<User *>			getAllUsers() const;
		std::string					getModeStr() const;
		bool						get_topic_exist() const;
		
		User *						getUser(std::string name);
		std::string					get_password() const;
		std::string					getTopic() const;

		bool						isInviteOnly() const;
		bool						hasTopicRestriction() const;

		std::string getCreationTime() const ;


		std::string					format_names();
		/* Setters */

		void						set_creationTime(std::time_t);
		void						set_key(std::string key);
		void						set_limit(size_t limit);
		void						set_ext_msg(bool flag);
		void						setTopic(User * user, std::string topic);

		/* Functions */

		void						broadcast(const std::string& message);
		void						broadcastIgnore(const std::string& message, User * userToIgnore);
		void						addUser(User *user);
		void  						addSuperUser(User *user);
		void						deleteUser(User * user);
		void						deleteOperator(User * user);
		void						deleteInvitation(User * user);
		void						kickUser(User * user, User * target, std::string reason);
		void					    inviteUser(User * user, User * target);

		void						broadcastMessage(const std::string& message);
		bool						isUserOperator(User * user);
		bool						isUserInvited(User * user);
		void						setModes(Server * srv, User * user, std::string mode, std::vector<std::string> modeArgs);
		bool						isValidMode(User * user, std::string mode);
		void						setLimit(std::string limitStr);
		void						setOperator(Server * srv, User * user, std::string target, bool addingMode);
};

#endif