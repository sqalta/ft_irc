#include "../Core/Includes/Server.hpp"
#include "ValidationUtils.hpp"
#include "NumericReplies.hpp"

void Server::Part(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "PART"));
        return;
    }

    std::string channelName = commands[j + 1];
    std::string message = commands.size() > 2 ? commands[j + 2] : "Leaving";
    int channelIndex = getChannelIndex(channelName);

    if (channelIndex == -1) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    std::vector<Client> channelClients = channels[channelIndex].getClients();
    if (!ValidationUtils::validateUserInChannel(channels[channelIndex], clients[id].getNickName())) {
        clients[id].print(ERR_NOTONCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    std::string partMessage = ":" + clients[id].getNickName() + "!" + 
                            clients[id].getUserName() + "@" + 
                            clients[id].getIp() + " PART " + 
                            channelName + " :" + 
                            message + "\r\n";

    // Kullanıcı operatör ise ve son operatör ise
    bool wasLastOperator = false;
    if (channels[channelIndex].isAdmin(clients[id].getNickName())) {
        channels[channelIndex].removeAdmin(clients[id].getNickName());
        
        // Başka operatör var mı kontrol et
        bool hasOtherOperator = false;
        for (size_t i = 0; i < channelClients.size(); i++) {
            if (channelClients[i].getNickName() != clients[id].getNickName() && 
                channels[channelIndex].isAdmin(channelClients[i].getNickName())) {
                hasOtherOperator = true;
                break;
            }
        }
        
        // Son operatör ise ve başka kullanıcı varsa
        if (!hasOtherOperator && channelClients.size() > 1) {
            wasLastOperator = true;
        }
    }

    // Part mesajını gönder
    for (size_t k = 0; k < channelClients.size(); k++) {
        channelClients[k].print(partMessage);
    }

    // Kullanıcıyı kanaldan çıkar
    channels[channelIndex].removeClient(clients[id].getNickName());
    clients[id].removeChannel(channelName);

    // Son operatör çıktıysa ve başka kullanıcı varsa
    if (wasLastOperator) {
        // Kanalda kalan ilk kullanıcıyı operatör yap
        std::vector<Client> remainingClients = channels[channelIndex].getClients();
        if (!remainingClients.empty()) {
            channels[channelIndex].addAdmin(remainingClients[0]);
            std::string modeMessage = ":" + serverName + " MODE " + channelName + 
                                    " +o " + remainingClients[0].getNickName() + "\r\n";
            
            // Mode değişikliğini bildir
            for (size_t i = 0; i < remainingClients.size(); i++) {
                remainingClients[i].print(modeMessage);
            }
        }
    }

    // Kanal boşsa kanalı sil
    if (channels[channelIndex].getClients().empty()) {
        channels.erase(channels.begin() + channelIndex);
    }
}