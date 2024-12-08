#ifndef NUMERIC_REPLIES_HPP
#define NUMERIC_REPLIES_HPP

#define RPL_WELCOME(client) ("001 " + client + " :Welcome to the Internet Relay Network " + client)
#define RPL_NAMREPLY(client, channel, names) ("353 " + client + " = " + channel + " :" + names)
#define RPL_ENDOFNAMES(client, channel) ("366 " + client + " " + channel + " :End of /NAMES list")
#define RPL_ENDOFWHO(client, channel) ("315 " + client + " " + channel + " :End of WHO list")

#define ERR_NOSUCHNICK(client, nick) ("401 " + client + " " + nick + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel")
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel")
#define ERR_ALREADYREGISTERED(client) ("462 " + client + " :You may not reregister")
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect")
#define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters")
#define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick + " " + channel + " :is already on channel")
#define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + client + " " + channel + " :You're not channel operator")

#define RPL_CHANNELMODEIS(nick, channel, modes) ("324 " + nick + " " + channel + " " + modes + "\r\n")
#define ERR_USERNOTINCHANNEL(source, target, channel) \
    ":" + serverName + " 441 " + source + " " + target + " " + channel + " :They aren't on that channel\r\n"
#define ERR_CANNOTKICKOP(source, channel) \
    ":" + serverName + " 485 " + source + " " + channel + " :Cannot kick another channel operator\r\n"
#endif