#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Kick(size_t j, int id)
{
    (void)j;
    
    if (!ValidationUtils::validateCommandFormat(commands, 3)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "KICK"));
        return;
    }

    std::string channelName = commands[1];
    std::string targetNick = commands[2];
    if (!ValidationUtils::validateChannelName(channelName)) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    int channelIndex = getChannelIndex(channelName);
    if (channelIndex == -1) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    if (!ValidationUtils::validateAdminPrivileges(channels[channelIndex], clients[id].getNickName())) {
        clients[id].print(ERR_CHANOPRIVSNEEDED(clients[id].getNickName(), channelName));
        return;
    }

    Channel* channel = &channels[channelIndex];

    if (!channel->isAdmin(clients[id].getNickName())) {
        clients[id].print(ERR_CHANOPRIVSNEEDED(clients[id].getNickName(), channelName));
        return;
    }

    if (!ValidationUtils::validateUserInChannel(*channel, targetNick)) {
        clients[id].print(ERR_USERNOTINCHANNEL(clients[id].getNickName(), targetNick, channelName));
        return;
    }

    if (channel->isAdmin(targetNick)) {
        clients[id].print(ERR_CANNOTKICKOP(clients[id].getNickName(), channelName));
        return;
    }

    std::string targetUser = commands[2];
    if (!ValidationUtils::validateUserInChannel(channels[channelIndex], targetUser)) {
        clients[id].print(ERR_NOSUCHNICK(clients[id].getNickName(), targetUser));
        return;
    }
    std::vector<Client> channelClients = channels[channelIndex].getClients();
    int targetUserIndex = getClientIndex2(targetUser, channelClients);
    
    if (targetUserIndex == -1)
    {
        clients[id].print("ERROR: Target user not found in channel\n");
        return;
    }

    std::string kickMessage;
    for (size_t i = 3; i < commands.size(); i++)
    {
        kickMessage += commands[i];
        kickMessage += " ";
    }

    for (size_t i = 0; i < channelClients.size(); i++)
    {
        channelClients[i].print(":" + clients[id].getNickName() + "!" + 
                               clients[id].getUserName() + "@" + 
                               clients[id].getIp() + " KICK " + 
                               channelName + " " + 
                               targetUser + " :" + 
                               kickMessage + "\r\n");
    }

    channelClients.erase(channelClients.begin() + targetUserIndex);
    channels[channelIndex].setClients(channelClients);
}