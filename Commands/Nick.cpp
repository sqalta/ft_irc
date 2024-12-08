#include "../Server/Server.hpp"
#include "../Commands/ValidationUtils.hpp"
#include "../Commands/NumericReplies.hpp"

void Server::Nick(size_t j, int id) {
    if (!ValidationUtils::validateCommandFormat(commands, 2)) {
        clients[id].print(ERR_NEEDMOREPARAMS(clients[id].getNickName(), "NICK"));
        return;
    }
    
    std::string newNick = commands[j + 1];
    if (ValidationUtils::validateUserExists(clients, newNick)) {
        clients[id].print(ERR_NOSUCHNICK(clients[id].getNickName(), newNick));
        return;
    }
    
    clients[id].setNickName(newNick);
    clients[id].setNickOK(true);
    checkRegistration(id);
}