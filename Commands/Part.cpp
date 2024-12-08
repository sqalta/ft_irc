#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Part(size_t j, int id)
{
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "PART"));
        return;
    }

    std::string channelName = commands[j + 1];
    if (!ValidationUtils::validateChannelName(channelName)) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    int channelIndex = getChannelIndex(channelName);
    if (channelIndex == -1) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    if (!ValidationUtils::validateUserInChannel(channels[channelIndex], clients[id].getNickName())) {
        clients[id].print(ERR_NOTONCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    std::string message;
    for (size_t i = 2; i < commands.size(); i++) {
        message += commands[i];
        message += " ";
    }

    std::string partMessage = ":" + clients[id].getNickName() + "!" + 
                            clients[id].getUserName() + "@" + 
                            clients[id].getIp() + " PART " + 
                            channelName + " :" + 
                            message + "\r\n";

    // Kanaldaki tüm kullanıcılara mesaj gönder
    std::vector<Client> channelClients = channels[channelIndex].getClients();
    for (size_t k = 0; k < channelClients.size(); k++) {
        channelClients[k].print(partMessage);
    }

    // Kullanıcıyı kanaldan çıkar
    channelClients.erase(channelClients.begin() + 
                        getClientIndex2(clients[id].getNickName(), channelClients));
    channels[channelIndex].setClients(channelClients);
    
    // Kullanıcının joinedChannels listesinden kanalı kaldır
    clients[id].removeChannel(channelName);
}