#include "Server.hpp"

Server::Server(int port, const std::string& pass) 
    : port(port), pass(pass), serverName("ft_irc") {
    initializeCommandMap();
    initilizeServer();
    runServer();
}

Server::~Server() {
    close(sockfd);
    for (size_t i = 0; i < clients.size(); ++i) {
        close(clients[i].getSocket());
    }
    channels.clear();
    clients.clear();
    std::cout << "[INFO] Server destroyed" << std::endl;
}

void Server::initializeCommandMap() {
    commandMap[USER] = &Server::User;
    commandMap[NICK] = &Server::Nick;
    commandMap[PASS] = &Server::Pass;
    commandMap[JOIN] = &Server::Join;
    commandMap[PRIVMSG] = &Server::Privmsg;
    commandMap[WHO] = &Server::Who;
    commandMap[KICK] = &Server::Kick;
    commandMap[PART] = &Server::Part;
    commandMap[QUIT] = &Server::Quit;
    commandMap[TOPIC] = &Server::Topic;
    commandMap[NOTICE] = &Server::Notice;
    commandMap[MODE] = &Server::Mode;
}

void Server::initilizeServer() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == SOCKET_ERROR)
        perr("[ERROR] Failed to create socket", sockfd);
    std::cout << "[INFO] Socket created successfully" << std::endl;

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        perr("[ERROR] Failed to set socket options", sockfd);

    setNonBlocking(sockfd);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        perr("[ERROR] Failed to bind socket", sockfd);
    std::cout << "[INFO] Server bound to port " << port << std::endl;

    if (listen(sockfd, 10) == SOCKET_ERROR)
        perr("[ERROR] Failed to listen on socket", sockfd);
    std::cout << "[INFO] IRC Server is listening for connections" << std::endl;
}

void Server::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        perr("[ERROR] Failed to get socket flags", fd);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        perr("[ERROR] Failed to set non-blocking mode", fd);
}

void Server::runServer() {
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;
    pollfds.push_back(pfd);

    while (true) {
        int ready = poll(&pollfds[0], pollfds.size(), -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            perror("[ERROR] Poll failed");
            break;
        }

        if (pollfds[0].revents & POLLIN) {
            handleNewConnection();
        }

        for (size_t i = 1; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                handleClientData(i);
            }
        }
    }
}

void Server::handleNewConnection() {
    int new_socket = accept(sockfd, NULL, NULL);
    if (new_socket >= 0) {
        setNonBlocking(new_socket);
        
        struct pollfd pfd;
        pfd.fd = new_socket;
        pfd.events = POLLIN;
        pollfds.push_back(pfd);

        Client newClient;
        newClient.setSocket(new_socket);
        clients.push_back(newClient);
        
        std::cout << "[INFO] New client connected. Total clients: " 
                  << clients.size() << std::endl;
    }
}

void Server::handleClientData(size_t clientIndex) {
    char buffer[BUFFER_SIZE] = {0};
    int valread = recv(pollfds[clientIndex].fd, buffer, BUFFER_SIZE - 1, 0);

    if (valread >= BUFFER_SIZE) {
        handleClientDisconnect(clientIndex);
        return;
    }
    if (valread > 0) {
        buffer[valread] = '\0';
        checkCommands(*this, buffer, pollfds[clientIndex].fd);
        logCommand(buffer, clientIndex - 1);
        processCommand(clientIndex - 1);
    }
    else if (valread == 0 || (valread < 0 && errno != EAGAIN)) {
        handleClientDisconnect(clientIndex);
    }
}

void Server::handleClientDisconnect(size_t index) {
    int clientId = index - 1;
    
    // Socket'i kapat
    close(pollfds[index].fd);
    
    // Quit mesajını gönder ve cleanup yap
    if (clientId >= 0 && clientId < static_cast<int>(clients.size())) {
        Quit(0, clientId);
        clients.erase(clients.begin() + clientId);
    }
    
    // Pollfd'den temizle
    pollfds.erase(pollfds.begin() + index);
    
    std::cout << "[INFO] Client disconnected. Remaining clients: " 
              << clients.size() << std::endl;
}

void Server::processCommand(int clientId) {
    if (commands.empty()) return;

    std::map<std::string, CommandFunction>::iterator it = commandMap.find(commands[0]);
    if (it != commandMap.end()) {
        // PASS, NICK ve USER komutlarını her zaman kabul et
        if (commands[0] == PASS || commands[0] == USER || commands[0] == NICK) {
            (this->*(it->second))(0, clientId);
            // Her komuttan sonra kayıt durumunu kontrol et
            checkRegistration(clientId);
        }
        // Diğer komutlar için login kontrolü yap
        else if (clients[clientId].getLoggedIn()) {
            (this->*(it->second))(0, clientId);
        }
        else {
            clients[clientId].print("451 :You need to complete the registration\r\n");
        }
    }
}

std::vector<Channel> Server::getChannels() { return channels; }
std::vector<Client> Server::getClients() { return clients; }
std::vector<std::string> Server::getCommands() { return commands; }

int Server::perr(const std::string& err, int sockfd) {
    std::cout << err << std::endl;
    close(sockfd);
    exit(-1);
}

int Server::getClientIndex(const std::string& name) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].getNickName() == name)
            return i;
    }
    return -1;
}

int Server::getClientIndex2(const std::string& name, const std::vector<Client>& clients) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].getNickName() == name)
            return i;
    }
    return -1;
}

void Server::checkCommands(Server& server, const std::string& buffer, int socket) {
    std::stringstream ss(buffer);
    (void)server;
    (void)socket;
    std::string line;
    commands.clear();

    while(std::getline(ss, line)) {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(" :\r\n", prev)) != std::string::npos) {
            if (pos > prev)
                commands.push_back(line.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < line.length())
            commands.push_back(line.substr(prev, std::string::npos));
    }
}

void Server::logCommand(const std::string& command, int clientId) {
    std::cout << "\033[1;34m[LOG]\033[0m Client " << clientId 
              << " -> Command: " << command;
}

int Server::isInChannel(const std::vector<Client>& clients, const std::string& nickname) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].getNickName() == nickname)
            return i;
    }
    return -1;
}

int Server::getChannelIndex(const std::string& channelName) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i].getChannelName() == channelName)
            return i;
    }
    return -1;
}

void Server::checkRegistration(int id) {
    if (clients[id].isRegistered() && !clients[id].getLoggedIn()) {
        std::string nick = clients[id].getNickName();
        clients[id].setLoggedIn(true);
        clients[id].setRegistered(true);
        
        clients[id].print("001 " + nick + " :Welcome to the IRC Network, " + nick + "\r\n");
        clients[id].print("002 " + nick + " :Your host is " + serverName + ", running version 1.0\r\n");
        clients[id].print("003 " + nick + " :This server was created today\r\n");
        clients[id].print("004 " + nick + " " + serverName + " 1.0 o o\r\n");
        clients[id].print("375 " + nick + " :- " + serverName + " Message of the day -\r\n");
        clients[id].print("376 " + nick + " :End of /MOTD command\r\n");

        std::cout << "[INFO] Client " << id << " (" << nick 
                  << ") completed registration" << std::endl;
    }
}

void Server::broadcastToChannel(const Channel& channel, const std::string& message) {
    const std::vector<Client>& channelClients = channel.getClients();
    for (size_t i = 0; i < channelClients.size(); i++) {
        int clientId = getClientIndex(channelClients[i].getNickName());
        if (clientId != -1) {
            clients[clientId].print(message);
        }
    }
}