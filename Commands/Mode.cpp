#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Mode(size_t j, int id)
{
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "MODE"));
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

    Channel* channel = &channels[channelIndex];

    // MODE #channel b gibi durumlar için
    if (commands.size() == 3) {
        std::string mode = commands[j + 2];
        if (mode == "b") {
            clients[id].print(":" + serverName + " 368 " + clients[id].getNickName() + 
                            " " + channelName + " :End of channel ban list\r\n");
            return;
        }
    }

    // MODE #channel +o/-o nick için
    if (commands.size() >= 4) {
        if (!ValidationUtils::validateAdminPrivileges(*channel, clients[id].getNickName())) {
            clients[id].print(ERR_CHANOPRIVSNEEDED(clients[id].getNickName(), channelName));
            return;
        }

        std::string mode = commands[j + 2];
        std::string targetNick = commands[j + 3];
        int targetIndex = getClientIndex(targetNick);

        if (targetIndex == -1) {
            clients[id].print(ERR_NOSUCHNICK(clients[id].getNickName(), targetNick));
            return;
        }

        // Hedef kullanıcı kanalda mı kontrol et
        if (!ValidationUtils::validateUserInChannel(*channel, targetNick)) {
            clients[id].print(ERR_USERNOTINCHANNEL(clients[id].getNickName(), targetNick, channelName));
            return;
        }

        if (mode == "+o") {
            // Kendine op verme kontrolü
            if (targetNick == clients[id].getNickName()) {
                clients[id].print(":" + serverName + " 481 " + clients[id].getNickName() + 
                                " :Cannot give operator status to yourself\r\n");
                return;
            }

            // Zaten operator mı kontrolü
            if (channel->isAdmin(targetNick)) {
                clients[id].print(":" + serverName + " 482 " + clients[id].getNickName() + 
                                " " + channelName + " :User already has operator status\r\n");
                return;
            }

            channel->addAdmin(clients[targetIndex]);
            std::string modeMessage = ":" + clients[id].getNickName() + " MODE " + 
                                    channel->getChannelName() + " +o " + targetNick + "\r\n";
            
            std::vector<Client> channelClients = channel->getClients();
            for (size_t i = 0; i < channelClients.size(); i++) {
                channelClients[i].print(modeMessage);
            }
        }
        else if (mode == "-o") {
            // Kendini deop yapma kontrolü
            if (targetNick == clients[id].getNickName()) {
                clients[id].print(":" + serverName + " 481 " + clients[id].getNickName() + 
                                " :Cannot remove your own operator status\r\n");
                return;
            }

            // Hedef operator değilse hata ver
            if (!channel->isAdmin(targetNick)) {
                clients[id].print(":" + serverName + " 482 " + clients[id].getNickName() + 
                                " " + channelName + " :User is not an operator\r\n");
                return;
            }

            // Başka bir operatörü deop yapma kontrolü
            if (channel->isAdmin(targetNick) && clients[id].getNickName() != targetNick) {
                clients[id].print(":" + serverName + " 482 " + clients[id].getNickName() + 
                                " " + channelName + " :Cannot remove operator status from another operator\r\n");
                return;
            }

            channel->removeAdmin(targetNick);
            std::string modeMessage = ":" + clients[id].getNickName() + " MODE " + 
                                    channel->getChannelName() + " -o " + targetNick + "\r\n";
            
            std::vector<Client> channelClients = channel->getClients();
            for (size_t i = 0; i < channelClients.size(); i++) {
                channelClients[i].print(modeMessage);
            }
        }
    }
}