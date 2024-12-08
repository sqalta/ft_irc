#include "../Server/Server.hpp"

void Server::Topic(size_t j, int id)
{
    if (commands.size() < 2)
    {
        clients[id].print("ERROR: Usage format is 'TOPIC #channelName [topic]'\n");
        return;
    }

    if (commands[j + 1][0] != '#')
    {
        clients[id].print("ERROR: Channel name must start with #\n");
        return;
    }

    int channelIndex = getChannelIndex(commands[j + 1]);
    if (channelIndex == -1)
    {
        clients[id].print("ERROR: Channel not found\n");
        return;
    }

    Channel& channel = channels[channelIndex];
    if (isInChannel(channel.getChannelAdmins(), clients[id].getNickName()) == -1)
    {
        clients[id].print("ERROR: You are not an admin in " + channel.getChannelName() + "\n");
        return;
    }

    if (commands.size() < 3)
    {
        clients[id].print("ERROR: Please provide a topic\n");
        return;
    }

    std::string topicMessage = commands[j + 2];
    std::vector<Client> channelClients = channel.getClients();
    for (size_t i = 0; i < channelClients.size(); i++)
    {
        channelClients[i].print("TOPIC " + channel.getChannelName() + " :" + topicMessage + "\r\n");
    }
}