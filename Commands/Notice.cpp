#include "../Server/Server.hpp"

void Server::Notice(size_t j, int id)
{
    (void)j;
    if (commands.size() < 3)
    {
        return; // NOTICE'de hata mesajı dönülmez (RFC 2812)
    }

    std::string target = commands[1];
    std::string message;
    for (size_t i = 2; i < commands.size(); i++)
    {
        message += commands[i];
        if (i < commands.size() - 1) {
            message += " ";
        }
    }

    // Kanal hedefli NOTICE
    if (target[0] == '#') {
        int channelIndex = getChannelIndex(target);
        if (channelIndex == -1) {
            return;
        }

        if (isInChannel(channels[channelIndex].getClients(), clients[id].getNickName()) == -1) {
            return;
        }

        std::vector<Client> channelClients = channels[channelIndex].getClients();
        for (size_t i = 0; i < channelClients.size(); i++) {
            if (channelClients[i].getNickName() != clients[id].getNickName()) {
                channelClients[i].print(":" + clients[id].getNickName() + "!" + 
                                    clients[id].getUserName() + "@" + 
                                    clients[id].getIp() + " NOTICE " + 
                                    target + " :" + message + "\r\n");
            }
        }
    }
    // Kullanıcı hedefli NOTICE
    else {
        for (size_t i = 0; i < clients.size(); i++) {
            if (clients[i].getNickName() == target) {
                clients[i].print(":" + clients[id].getNickName() + "!" + 
                               clients[id].getUserName() + "@" + 
                               clients[id].getIp() + " NOTICE " + 
                               target + " :" + message + "\r\n");
                return;
            }
        }
    }
}