#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include "Server.hpp"

class Server;

bool						hasCarriageReturn(std::string str);
std::string					cpyWithoutCR(std::string str);
void						printArgs(std::vector<std::string> args);
std::vector<std::string>	splitString(std::string input);
bool						isValidNickName(std::string nickName);
std::string					findAvailableNickName(Server * srv, std::string nickName);
std::string 				buffersConcat(std::vector<std::string> buffers);

#endif