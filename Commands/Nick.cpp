#include "../Core/Includes/Server.hpp"
#include "ValidationUtils.hpp"
#include "NumericReplies.hpp"

void Server::Nick(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "NICK"));
        return;
    }
    
    std::string newNick = commands[j + 1];
    
    // Nickname kullanımda mı kontrol et
    for (size_t i = 0; i < clients.size(); i++) {
        if (static_cast<int>(i) != id && clients[i].getNickName() == newNick) {
            clients[id].print("433 " + clients[id].getNickName() + " " + 
                            newNick + " :Nickname is already in use\r\n");
            return;
        }
    }
    
    // Nick değişiklik mesajını hazırla
    std::string nickChangeMsg = ":" + clients[id].getNickName() + "!" +
                              clients[id].getUserName() + "@" +
                              clients[id].getIp() + " NICK :" + newNick + "\r\n";
    
    // Eğer kullanıcı kanallardaysa, nick değişikliğini kanallara bildir
    std::vector<std::string> userChannels = clients[id].getJoinedChannels();
    std::vector<std::string> notifiedUsers;
    
    for (size_t i = 0; i < userChannels.size(); i++) {
        int channelIndex = getChannelIndex(userChannels[i]);
        if (channelIndex != -1) {
            std::vector<Client> channelClients = channels[channelIndex].getClients();
            for (size_t j = 0; j < channelClients.size(); j++) {
                bool alreadyNotified = false;
                for (size_t k = 0; k < notifiedUsers.size(); k++) {
                    if (notifiedUsers[k] == channelClients[j].getNickName()) {
                        alreadyNotified = true;
                        break;
                    }
                }
                if (!alreadyNotified) {
                    channelClients[j].print(nickChangeMsg);
                    notifiedUsers.push_back(channelClients[j].getNickName());
                }
            }
        }
    }
    
    // Kullanıcının kendisine de bildir
    bool selfNotified = false;
    for (size_t i = 0; i < notifiedUsers.size(); i++) {
        if (notifiedUsers[i] == clients[id].getNickName()) {
            selfNotified = true;
            break;
        }
    }
    if (!selfNotified) {
        clients[id].print(nickChangeMsg);
    }
    
    // Nick'i değiştir
    clients[id].setNickName(newNick);
    clients[id].setNickOK(true);
    
    // Eğer kayıt aşamasındaysa kayıt kontrolü yap
    if (!clients[id].getLoggedIn()) {
        completeUserRegistration(id);
    }
}