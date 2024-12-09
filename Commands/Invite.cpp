#include "../Core/Includes/Server.hpp"
#include "ValidationUtils.hpp"
#include "NumericReplies.hpp"

void Server::Invite(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 3)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "INVITE"));
        return;
    }

    std::string targetNick = commands[j + 1];
    std::string channelName = commands[j + 2];
    
    // Hedef kullanıcı var mı kontrol et
    int targetIndex = getClientIndex(targetNick);
    if (targetIndex == -1) {
        clients[id].print(ERR_NOSUCHNICK(clients[id].getNickName(), targetNick));
        return;
    }

    // Kanal var mı kontrol et
    int channelIndex = getChannelIndex(channelName);
    if (channelIndex == -1) {
        clients[id].print(ERR_NOSUCHCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    Channel* channel = &channels[channelIndex];

    // Davet eden kullanıcı kanalda mı kontrol et
    if (!ValidationUtils::validateUserInChannel(*channel, clients[id].getNickName())) {
        clients[id].print(ERR_NOTONCHANNEL(clients[id].getNickName(), channelName));
        return;
    }

    // Hedef kullanıcı zaten kanalda mı kontrol et
    if (ValidationUtils::validateUserInChannel(*channel, targetNick)) {
        clients[id].print(ERR_USERONCHANNEL(clients[id].getNickName(), targetNick, channelName));
        return;
    }

    // Davet eden kullanıcı operator mı kontrol et
    if (!channel->isAdmin(clients[id].getNickName())) {
        clients[id].print(ERR_CHANOPRIVSNEEDED(clients[id].getNickName(), channelName));
        return;
    }

    // Davet mesajlarını gönder
    std::string inviteMsg = ":" + clients[id].getNickName() + "!" + 
                           clients[id].getUserName() + "@" + 
                           clients[id].getIp() + " INVITE " + 
                           targetNick + " " + channelName + "\r\n";

    // Hedef kullanıcıya davet mesajı
    clients[targetIndex].print(inviteMsg);

    // Davet eden kullanıcıya onay mesajı
    clients[id].print(":" + serverName + " 341 " + clients[id].getNickName() + " " + 
                     targetNick + " " + channelName + "\r\n");
}