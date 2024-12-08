#ifndef VALIDATION_UTILS_HPP
#define VALIDATION_UTILS_HPP

#include <string>
#include <vector>
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"

class ValidationUtils {
public:
    // Genel validasyonlar
    static bool validateCommandFormat(const std::vector<std::string>& commands, size_t minParams);
    static bool validateChannelName(const std::string& channelName);
    
    // Kanal validasyonları
    static bool validateChannelExists(const std::vector<Channel>& channels, const std::string& channelName);
    static bool validateUserInChannel(const Channel& channel, const std::string& nickname);
    static bool validateAdminPrivileges(const Channel& channel, const std::string& nickname);
    static bool validateUserNotInChannel(const Channel& channel, const std::string& nickname);
    
    // Kullanıcı validasyonları
    static bool validateUserExists(const std::vector<Client>& clients, const std::string& nickname);
    static bool validatePassword(const std::string& inputPass, const std::string& serverPass);
    static bool validateRegistration(const Client& client);

    static bool canModifyOperatorStatus(const Channel& channel, 
                                  const std::string& sourceNick, 
                                  const std::string& targetNick);

private:
    ValidationUtils();
};

#endif