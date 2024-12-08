#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include "Client.hpp"
#include "Channel.hpp"

#define SOCKET_ERROR -1
#define BUFFER_SIZE 1024

// Komut tanımlamaları
#define PASS "PASS"
#define NICK "NICK"
#define USER "USER"
#define JOIN "JOIN"
#define PRIVMSG "PRIVMSG"
#define WHO "WHO"
#define KICK "KICK"
#define PART "PART"
#define QUIT "QUIT"
#define TOPIC "TOPIC"
#define NOTICE "NOTICE"
#define MODE "MODE"

class Server {
private:
    typedef void (Server::*CommandFunction)(size_t, int);
    std::map<std::string, CommandFunction> commandMap;

    int sockfd;
    int port;
    std::string pass;
    struct sockaddr_in server_address;
    std::vector<Channel> channels;
    std::vector<Client> clients;
    std::vector<struct pollfd> pollfds;
    std::vector<std::string> commands;
    std::string serverName;

    void initializeCommandMap();
    void initializeMainSocket();
    void startListening();
	void initializePoll();
	int waitForEvents();
	void checkAndProcessEvents();
	void processMainSocketEvents();
	void processClientSocketEvents(size_t clientIndex);
    void createNewConnection();
    void handleClientEvent(size_t clientIndex);
    void disconnectClient(size_t index);
    void setNonBlocking(int fd);
    void processCommand(int clientId);
    void broadcastToChannel(const Channel& channel, const std::string& message);
    int getClientIdBySocket(int socket);
public:
    Server(int port, const std::string& pass);
    ~Server();

    // Getter metodları
    std::vector<Channel> getChannels();
    std::vector<Client> getClients();
    std::vector<std::string> getCommands();
    std::string getServerName() const { return serverName; };

    // Yardımcı metodlar
    int perr(const std::string& err, int sockfd);
    int getClientIndex(const std::string& name);
    int getClientIndexInList(const std::string& name, const std::vector<Client>& clients);
    void checkCommands(const std::string& buffer, int socket);
    int isInChannel(const std::vector<Client>& clients, const std::string& nickname);
    int getChannelIndex(const std::string& channelName);
    void logCommand(const std::string& command, int clientId);
    void checkRegistration(int id);
	std::vector<std::string> Server::parseLine(const std::string& line);

    // Komut işleme metodları
    void User(size_t j, int id);
    void Nick(size_t j, int id);
    void Pass(size_t j, int id);
    void Join(size_t j, int id);
    void Privmsg(size_t j, int id);
    void Who(size_t j, int id);
    void Kick(size_t j, int id);
    void Part(size_t j, int id);
    void Quit(size_t j, int id);
    void Topic(size_t j, int id);
    void Notice(size_t j, int id);
    void Mode(size_t j, int id);
};

#endif