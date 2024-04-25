/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nigarcia <nigarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:58:26 by nigarcia          #+#    #+#             */
/*   Updated: 2024/04/24 16:20:54 by nigarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool	hasCarriageReturn(std::string str)
{
	return (str.find(13) != std::string::npos);
}

std::string	cpyWithoutCR(std::string str) {

	std::string copy = "";
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] != 13)
			copy += str[i];
	}
	return (copy);
}


void	printArgs(std::vector<std::string> args)
{
	for (size_t i = 0; i < args.size() ;i++)
		std::cout << "args["<<i<<"] = " << args[i] << std::endl;
}


std::vector<std::string>	splitString(std::string input)
{
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string word;

    while (std::getline(iss, word, ',')) {
        result.push_back(word);
    }

    return result;
}

bool	isValidNickName(std::string nickName)
{
	
    // They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
    // They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
    // They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
    // They SHOULD NOT contain any dot character ('.', 0x2E).
	
	std::string forbidenChar = " ,*?!@.";
	std::string forbidenPrefix = "$:#&~@+%";

	if (nickName.empty())
		return (false);

	if (forbidenPrefix.find(nickName[0]) != std::string::npos)
		return (false);

	for (size_t i = 0; i < forbidenChar.size(); i++)
	{
		if (nickName.find(forbidenChar[i]) != std::string::npos)
			return (false);
	}

	return (true);
}

std::string intToString(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string	findAvailableNickName(Server * srv, std::string nickName)
{
	std::string newNickName = nickName;
	int suffix = 0;
	while (srv->getUserByNickname(newNickName) != NULL)
	{
		newNickName = nickName + intToString(suffix);
		suffix++;
	}
	return (newNickName);
}

std::string buffersConcat(std::vector<std::string> buffers)
{
	std::string concat = "";

	for (size_t i = 0; i < buffers.size(); i++)
	{
		concat += buffers[i];
	}
	return (concat);
}