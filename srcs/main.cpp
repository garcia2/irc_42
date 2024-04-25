/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jileroux <jileroux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:27:17 by jileroux          #+#    #+#             */
/*   Updated: 2024/04/24 14:27:19 by jileroux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return (EXIT_FAILURE);
    }
    try
    {
        int port = std::atoi(argv[1]);
        if (!port || port < 1024 || port > 65535)
            throw std::invalid_argument("Invalid port");
        Server ircserv(port, argv[2]);
        std::signal(SIGINT, ircserv.signalHandler);
        ircserv.run();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
	    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
