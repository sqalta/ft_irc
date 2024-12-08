#include "Client.hpp"

Client::Client() : 
    socket(-1), 
    pass(""), 
    nick_name(""), 
    user_name(""), 
    ip(""), 
    logged_in(false),
    isOperator(false),
    passOK(false),
    nickOK(false),
    userOK(false) {}

Client::~Client() {}

// Getters
int Client::getSocket() const { return socket; }
bool Client::getLoggedIn() const { return logged_in; }
std::string Client::getIp() const { return ip; }
std::string Client::getUserName() const { return user_name; }
std::string Client::getNickName() const { return nick_name; }
std::string Client::getPass() const { return pass; }
const std::vector<std::string>& Client::getJoinedChannels() const { return joinedChannels; }
bool Client::getIsOperator() const { return isOperator; }

// Setters
void Client::setSocket(int sockfd) { socket = sockfd; }
void Client::setUserName(std::string user_name) { this->user_name = user_name; }
void Client::setNickName(std::string nick_name) { this->nick_name = nick_name; }
void Client::setPass(std::string pass) { this->pass = pass; }
void Client::setLoggedIn(bool sign) { logged_in = sign; }
void Client::setIp(std::string ip) { this->ip = ip; }
void Client::setIsOperator(bool status) { isOperator = status; }

// Channel operations
void Client::joinChannel(const std::string& channelName) {
    if (!isInChannel(channelName)) {
        joinedChannels.push_back(channelName);
    }
}

void Client::leaveChannel(const std::string& channelName) {
    joinedChannels.erase(
        std::remove(joinedChannels.begin(), joinedChannels.end(), channelName),
        joinedChannels.end()
    );
}

bool Client::isInChannel(const std::string& channelName) const {
    return std::find(joinedChannels.begin(), joinedChannels.end(), channelName) 
           != joinedChannels.end();
}

void Client::print(const std::string& str) const { 
    size_t total = 0;
    while (total < str.size()) {
        ssize_t sent = send(socket, str.c_str() + total, str.size() - total, 0);
        if (sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            break;
        }
        total += sent;
    }
}

void Client::removeChannel(const std::string& channelName) {
    joinedChannels.erase(std::remove(joinedChannels.begin(), joinedChannels.end(), channelName), 
                        joinedChannels.end());
}