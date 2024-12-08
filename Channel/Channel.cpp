#include "Channel.hpp"

Channel::Channel(std::string name) : 
    channelName(name), 
    inviteOnly(false),
    topicRestricted(false),
    userLimit(0) {}

Channel::~Channel() {}

const std::vector<Client>& Channel::getClients() const {
    return clients;
}

const std::vector<Client>& Channel::getChannelAdmins() const {
    return channelAdmins;
}

std::string Channel::getChannelName() const {
    return channelName;
}

std::string Channel::getTopic() const {
    return topic;
}

void Channel::setClients(std::vector<Client> clients) {
    this->clients = clients;
}

void Channel::setChannelAdmins(std::vector<Client> channelAdmins) {
    this->channelAdmins = channelAdmins;
}

void Channel::setTopic(std::string new_topic) {
    topic = new_topic;
}

void Channel::addClient(Client& client) {
    clients.push_back(client);
}

void Channel::removeClient(const std::string& nickname) {
    std::vector<Client>::iterator it = clients.begin();
    while (it != clients.end()) {
        if (it->getNickName() == nickname) {
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

void Channel::addAdmin(Client admin) {
    channelAdmins.push_back(admin);
}

void Channel::removeAdmin(std::string nickname) {
    std::vector<Client>::iterator it = channelAdmins.begin();
    while (it != channelAdmins.end()) {
        if (it->getNickName() == nickname) {
            it = channelAdmins.erase(it);
        } else {
            ++it;
        }
    }
}

bool Channel::isAdmin(const std::string& nickname) const {
    std::vector<Client>::const_iterator it;
    for (it = channelAdmins.begin(); it != channelAdmins.end(); ++it) {
        if (it->getNickName() == nickname)
            return true;
    }
    return false;
}

bool Channel::isInvited(const std::string& nickname) const {
    return std::find(invitedUsers.begin(), invitedUsers.end(), nickname) != invitedUsers.end();
}

void Channel::addInvitedUser(const std::string& nickname) {
    if (!isInvited(nickname)) {
        invitedUsers.push_back(nickname);
    }
}