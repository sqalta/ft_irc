#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Privmsg(size_t j, int id)
{
    if (!ValidationUtils::validateCommandFormat(commands, 3)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "PRIVMSG"));
        return;
    }

    std::string target = commands[1];
    if (target[0] == '#') {
        if (!ValidationUtils::validateChannelName(target)) {
            clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), target));
            return;
        }

        int channelIndex = getChannelIndex(target);
        if (channelIndex == -1) {
            clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), target));
            return;
        }

        if (!ValidationUtils::validateUserInChannel(channels[channelIndex], clients[id].getNickName())) {
            clients[id].print(ERR_CANNOTSENDTOCHAN(clients[id].getNickName(), target));
            return;
        }
    }
	(void) j;
    if (commands.size() < 3)
    {
        clients[id].print("ERROR: Usage format is 'PRIVMSG <target> <message>'\n");
        return;
    }

    std::string message;
    for (size_t i = 2; i < commands.size(); i++)
    {
        message += commands[i];
        message += " ";
    }

    for (size_t i = 0; i < clients.size(); i++)
    {
        if (strcmp(clients[i].getNickName().c_str(), commands[1].c_str()) == 0)
        {
            clients[i].print(":" + clients[id].getNickName() + "!" + 
                           clients[id].getUserName() + "@" + 
                           clients[id].getIp() + " PRIVMSG " + 
                           clients[i].getNickName() + " :" + 
                           message + "\r\n");
            return;
        }
    }

    for (size_t i = 0; i < channels.size(); i++)
    {
        if (strcmp(commands[1].c_str(), channels[i].getChannelName().c_str()) == 0)
        {
            if (isInChannel(channels[i].getClients(), clients[id].getNickName()) == -1)
            {
                clients[id].print("ERROR: You are not in this channel\n");
                return;
            }

            std::vector<Client> channelClients = channels[i].getClients();
            for (size_t k = 0; k < channelClients.size(); k++)
            {
                if (channelClients[k].getNickName() != clients[id].getNickName())
                {
                    channelClients[k].print(":" + clients[id].getNickName() + "!" + 
                                          clients[id].getUserName() + "@" + 
                                          clients[id].getIp() + " PRIVMSG " + 
                                          channels[i].getChannelName() + " :" + 
                                          message + "\r\n");
                }
            }
            return;
        }
    }
    
    clients[id].print("ERROR: Target not found\n");
}