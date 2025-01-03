#include "ValidationUtils.hpp"

bool ValidationUtils::validateCommandFormat(const std::vector<std::string>& commands, size_t minParams) {
    return commands.size() >= minParams;
}

bool ValidationUtils::validateChannelName(const std::string& channelName) {
    return !channelName.empty() && channelName[0] == '#';
}

bool ValidationUtils::validateChannelExists(const std::vector<Channel>& channels, const std::string& channelName) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i].getChannelName() == channelName)
            return true;
    }
    return false;
}

bool ValidationUtils::validateUserInChannel(const Channel& channel, const std::string& nickname) {
    std::vector<Client> clients = channel.getClients();
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].getNickName() == nickname)
            return true;
    }
    return false;
}

bool ValidationUtils::validateUserNotInChannel(const Channel& channel, const std::string& nickname) {
    return !validateUserInChannel(channel, nickname);
}

bool ValidationUtils::validateAdminPrivileges(const Channel& channel, const std::string& nickname) {
    return channel.isAdmin(nickname);
}

bool ValidationUtils::validatePassword(const std::string& inputPass, const std::string& serverPass) {
    std::string cleanPass = inputPass;
    // ':' karakteri ile başlıyorsa kaldır
    if (!cleanPass.empty() && cleanPass[0] == ':') {
        cleanPass = cleanPass.substr(1);
    }
    return cleanPass == serverPass;
}

bool ValidationUtils::validateUserExists(const std::vector<Client>& clients, const std::string& nickname) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].getNickName() == nickname)
            return true;
    }
    return false;
}

bool ValidationUtils::canModifyOperatorStatus(const Channel& channel, 
                                            const std::string& sourceNick, 
                                            const std::string& targetNick) {
    if (sourceNick == targetNick) return false;
    if (!channel.isAdmin(sourceNick)) return false;
    if (channel.isAdmin(targetNick)) return false;
    return true;
}

bool ValidationUtils::validateNickFormat(const std::string& nick) {
    if (nick.empty() || nick.length() > 9)
        return false;
        
    // İlk karakter harf veya özel karakter olmalı
    if (!isalpha(nick[0]) && nick[0] != '[' && nick[0] != ']' && 
        nick[0] != '\\' && nick[0] != '`' && nick[0] != '_' && 
        nick[0] != '^' && nick[0] != '{' && nick[0] != '|' && nick[0] != '}')
        return false;
        
    // Diğer karakterler harf, rakam veya özel karakter olabilir
    for (size_t i = 1; i < nick.length(); i++) {
        if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' && 
            nick[i] != ']' && nick[i] != '\\' && nick[i] != '`' && 
            nick[i] != '_' && nick[i] != '^' && nick[i] != '{' && 
            nick[i] != '|' && nick[i] != '}')
            return false;
    }
    return true;
}