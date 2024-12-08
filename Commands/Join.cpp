#include "../Server/Server.hpp"
#include "../Commands/NumericReplies.hpp"
#include "../Commands/ValidationUtils.hpp"

void Server::Join(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "JOIN"));
        return;
    }

    std::string channelName = commands[j + 1];
    if (!ValidationUtils::validateChannelName(channelName)) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    // Kullanıcı zaten kanalda mı kontrol et
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->getChannelName() == channelName) {
            std::vector<Client> channelClients = it->getClients();
            for (size_t i = 0; i < channelClients.size(); i++) {
                if (channelClients[i].getNickName() == clients[id].getNickName()) {
                    clients[id].print("443 " + clients[id].getNickName() + " " + 
                                    channelName + " :is already on channel\r\n");
                    return;
                }
            }
        }
    }

    Channel* targetChannel = NULL;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->getChannelName() == channelName) {
            targetChannel = &(*it);
            break;
        }
    }

    std::string joinMessage = ":" + clients[id].getNickName() + "!" + 
                            clients[id].getUserName() + "@" + 
                            clients[id].getIp() + " JOIN " + 
                            channelName + "\r\n";

    if (targetChannel == NULL) {
        channels.push_back(Channel(channelName));
        targetChannel = &channels.back();
        targetChannel->addClient(clients[id]);
        targetChannel->addAdmin(clients[id]);
        clients[id].joinChannel(channelName);  // Client'ın kanal listesine ekle
        
        // Admin olduğunu bildir
        std::string modeMessage = ":" + clients[id].getNickName() + " MODE " + channelName + 
                                " +o " + clients[id].getNickName() + "\r\n";
        clients[id].print(modeMessage);
    } else {
        targetChannel->addClient(clients[id]);
        clients[id].joinChannel(channelName);  // Client'ın kanal listesine ekle
    }

    // Join mesajını tüm kanal üyelerine gönder
    std::vector<Client> channelClients = targetChannel->getClients();
    for (size_t i = 0; i < channelClients.size(); i++) {
        channelClients[i].print(joinMessage);
    }

    // Names listesini gönder
    std::string namesPrefix = ":" + serverName + " 353 " + clients[id].getNickName() + 
                            " = " + channelName + " :";
    std::string namesList;
    std::vector<Client> currentClients = targetChannel->getClients();
    for (size_t i = 0; i < currentClients.size(); ++i) {
        const Client& client = currentClients[i];
        std::string nickName = client.getNickName();
        if (targetChannel->isAdmin(nickName)) {
            namesList += "@" + nickName + " ";
        } else {
            namesList += nickName + " ";
        }
    }
    clients[id].print(namesPrefix + namesList + "\r\n");
    clients[id].print(":" + serverName + " 366 " + clients[id].getNickName() + " " + 
                    channelName + " :End of /NAMES list.\r\n");
}