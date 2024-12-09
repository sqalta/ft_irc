#include "../Core/Includes/Server.hpp"
#include "ValidationUtils.hpp"
#include "NumericReplies.hpp"

void Server::Who(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "WHO"));
        return;
    }

    std::string target = commands[j + 1];
    std::string operFlag = (commands.size() > 2 && commands[j + 2] == "o") ? "o" : "";

    // Kanal için WHO
    if (target[0] == '#') {
        int channelIndex = getChannelIndex(target);
        if (channelIndex == -1) {
            clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), target));
            return;
        }

        std::vector<Client> channelClients = channels[channelIndex].getClients();
        for (size_t i = 0; i < channelClients.size(); i++) {
            // Eğer 'o' flag'i varsa sadece operatörleri göster
            if (operFlag == "o" && !channels[channelIndex].isAdmin(channelClients[i].getNickName()))
                continue;

            std::string userFlags = "H"; // Here
            if (channels[channelIndex].isAdmin(channelClients[i].getNickName()))
                userFlags += "@";

            // RPL_WHOREPLY formatı: "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
            clients[id].print("352 " + clients[id].getNickName() + " " + 
                            target + " " +
                            channelClients[i].getUserName() + " " +
                            channelClients[i].getIp() + " " +
                            serverName + " " +
                            channelClients[i].getNickName() + " " +
                            userFlags + " :0 " +
                            channelClients[i].getUserName() + "\r\n");
        }
    }
    // Kullanıcı için WHO
    else {
        int targetIndex = getClientIndex(target);
        if (targetIndex != -1) {
            // Eğer 'o' flag'i varsa ve kullanıcı hiçbir kanalda operatör değilse atla
            bool isOperator = false;
            for (size_t i = 0; i < channels.size(); i++) {
                if (channels[i].isAdmin(target)) {
                    isOperator = true;
                    break;
                }
            }
            
            if (operFlag == "o" && !isOperator)
                return;

            std::string userFlags = "H";
            // Kullanıcının üye olduğu tüm kanalları kontrol et
            std::vector<std::string> userChannels = clients[targetIndex].getJoinedChannels();
            for (size_t i = 0; i < userChannels.size(); i++) {
                int chIndex = getChannelIndex(userChannels[i]);
                if (chIndex != -1 && channels[chIndex].isAdmin(target)) {
                    userFlags += "@";
                    break;
                }
            }

            clients[id].print("352 " + clients[id].getNickName() + " * " +
                            clients[targetIndex].getUserName() + " " +
                            clients[targetIndex].getIp() + " " +
                            serverName + " " +
                            clients[targetIndex].getNickName() + " " +
                            userFlags + " :0 " +
                            clients[targetIndex].getUserName() + "\r\n");
        }
    }

    // RPL_ENDOFWHO
    clients[id].print("315 " + clients[id].getNickName() + " " + target + " :End of WHO list\r\n");
}