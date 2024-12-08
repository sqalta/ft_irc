#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "../Client/Client.hpp"

class Channel {
private:
    std::string channelName;
    std::string topic;
    std::vector<Client> clients;
    std::vector<Client> channelAdmins;
    bool inviteOnly;
    bool topicRestricted;
    std::string key;
    size_t userLimit;
    std::vector<std::string> invitedUsers;

public:
    Channel(std::string name);
    ~Channel();

    // Getters
    const std::vector<Client>& getClients() const;
    const std::vector<Client>& getChannelAdmins() const;
    std::string getChannelName() const;
    std::string getTopic() const;
    bool getInviteOnly() const { return inviteOnly; }
    bool getTopicRestricted() const { return topicRestricted; }
    std::string getKey() const { return key; }
    size_t getUserLimit() const { return userLimit; }

    // Setters
    void setClients(std::vector<Client> clients);
    void setChannelAdmins(std::vector<Client> channelAdmins);
    void setTopic(std::string new_topic);
    void setInviteOnly(bool value) { inviteOnly = value; }
    void setTopicRestricted(bool value) { topicRestricted = value; }
    void setKey(const std::string& newKey) { key = newKey; }
    void setUserLimit(size_t limit) { userLimit = limit; }

    // Member functions
    void addClient(Client& client);
    void removeClient(const std::string& nickname);
    void addAdmin(Client admin);
    void removeAdmin(std::string nickname);
    bool isAdmin(const std::string& nickname) const;
    bool isInvited(const std::string& nickname) const;
    void addInvitedUser(const std::string& nickname);
};

#endif