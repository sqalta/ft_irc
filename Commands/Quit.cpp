#include "../Server/Server.hpp"

void Server::Quit(size_t j, int id) {
    (void)j;
    std::string quitMessage = ":" + clients[id].getNickName() + "!" + 
                             clients[id].getUserName() + "@" + 
                             clients[id].getIp() + " QUIT :Leaving\r\n";

    // Tüm kanallara quit mesajını gönder
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end();) {
        std::vector<Client> channelClients = it->getClients();
        int clientIndex = isInChannel(channelClients, clients[id].getNickName());
        
        if (clientIndex != -1) {
            // Quit mesajını kanaldaki diğer kullanıcılara gönder
            for (size_t k = 0; k < channelClients.size(); k++) {
                if (channelClients[k].getNickName() != clients[id].getNickName()) {
                    channelClients[k].print(quitMessage);
                }
            }
            
            // Kullanıcıyı kanaldan çıkar
            it->removeClient(clients[id].getNickName());
            
            // Kanal boşsa kanalı sil
            if (it->getClients().empty()) {
                it = channels.erase(it);
                continue;
            }
        }
        ++it;
    }

    // Client'a quit mesajını gönder ve bağlantıyı kapat
    clients[id].print(quitMessage);
    close(clients[id].getSocket());
}