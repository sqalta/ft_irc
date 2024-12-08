#include "../Core/Includes/Server.hpp"
#include "ValidationUtils.hpp"
#include "NumericReplies.hpp"

void Server::Who(size_t j, int id)
{
    (void)j;
    
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "WHO"));
        return;
    }

    std::string channelName = commands[1];
    if (!ValidationUtils::validateChannelName(channelName)) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    int channelIndex = getChannelIndex(channelName);
    if (channelIndex == -1) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    std::vector<Client> channelClients = channels[channelIndex].getClients();
    for (size_t i = 0; i < channelClients.size(); i++)
    {
        std::string userFlags = channels[channelIndex].isAdmin(channelClients[i].getNickName()) ? "@" : "";
        clients[id].print("352 " + clients[id].getNickName() + " " + channelName + " " + 
                         channelClients[i].getUserName() + " " + channelClients[i].getIp() + " " + 
                         serverName + " " + channelClients[i].getNickName() + " " + 
                         userFlags + " :0 " + channelClients[i].getUserName() + "\r\n");
    }
    
    clients[id].print(RPL_ENDOFWHO(clients[id].getNickName(), channelName));
}