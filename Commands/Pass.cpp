#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Pass(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "PASS"));
        return;
    }

    if (clients[id].isRegistered()) {
        clients[id].print(ERR_ALREADYREGISTERED(clients[id].getNickName()));
        return;
    }

    if (!ValidationUtils::validatePassword(commands[j + 1], pass)) {
        clients[id].print(ERR_PASSWDMISMATCH(clients[id].getNickName()));
        return;
    }

    clients[id].setPass(commands[j + 1]);
    clients[id].setPassOK(true);
    checkRegistration(id);
}