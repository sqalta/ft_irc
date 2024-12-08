#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <sys/socket.h>
#include <errno.h>

class Client {
private:
    int socket;
    std::string pass;
    std::string nick_name;
    std::string user_name;
    std::string ip;
    bool logged_in;
    std::vector<std::string> joinedChannels;
    bool isOperator;
    bool passOK;
    bool nickOK;
    bool userOK;
    bool registered;

public:
    Client();
    void setPassOK(bool status) { passOK = status; }
    void setNickOK(bool status) { nickOK = status; }
    void setUserOK(bool status) { userOK = status; }
    void setRegistered(bool status) { registered = status; }
    bool isRegistered() const {
    return passOK && nickOK && userOK;
}
    ~Client();

    // Getters
    int getSocket() const;
    bool getLoggedIn() const;
    std::string getIp() const;
    std::string getUserName() const;
    std::string getNickName() const;
    std::string getPass() const;
    const std::vector<std::string>& getJoinedChannels() const;
    bool getIsOperator() const;

    // Setters
    void setSocket(int sockfd);
    void setUserName(std::string user_name);
    void setNickName(std::string nick_name);
    void setPass(std::string pass);
    void setLoggedIn(bool sign);
    void setIp(std::string ip);
    void setIsOperator(bool status);

    // Channel operations
    void joinChannel(const std::string& channelName);
    void leaveChannel(const std::string& channelName);
    bool isInChannel(const std::string& channelName) const;
    void removeChannel(const std::string& channelName);

    // Communication
    void print(const std::string& str) const;
};

#endif