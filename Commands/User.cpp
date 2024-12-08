#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::User(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 4)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "USER"));
        return;
    }

    if (clients[id].isRegistered()) {
        clients[id].print(ERR_ALREADYREGISTERED(clients[id].getNickName()));
        return;
    }

    clients[id].setUserName(commands[j + 1]);
    clients[id].setIp(commands[j + 3]);
    clients[id].setUserOK(true);
    checkRegistration(id);
}