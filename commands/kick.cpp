#include "../commands.hpp"

int kick(IRCServer &server, Client &client, std::vector<std::string> &arguments) 
{
    if (!client.getAuth() || !client.getRegistered()) {
        client.msg(ERR_NOTREGISTERED(client.getNickname(), "KICK"));
        return 0;
    }

    if (arguments.size() < 3) {
        client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
        return 0;
    }

    std::string channelName = arguments[1].substr(1); 
    Channel *channel = server.getChannelByName(channelName);
    if (channel == NULL) {
        client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
        return 0;
    }

    if (!client.isInChannel(channelName)) {
        client.msg(ERR_NOTONCHANNEL(client.getNickname(), arguments[1]));
        return 0;
    }

    if (!channel->isOperator(&client)) {
        client.msg(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return 0;
    }

    Client *clientToKick = channel->getClientByNickname(arguments[2]);
    if (clientToKick == NULL) {
        client.msg(ERR_USERNOTINCHANNEL(client.getNickname(), channelName, arguments[2]));
        return 0;
    }

    std::string kickReason = "No reason specified";
    if (arguments.size() > 3) {
        // Combine the remaining arguments vector into a single string for the reason
        std::vector<std::string>::const_iterator it;
        kickReason = "";
        for (it = arguments.begin() + 3; it != arguments.end(); ++it) {
            if (it != arguments.begin() + 3) {
                kickReason += " ";
            }
            kickReason += *it;
        }
    }

    std::string kickMessage = KICK(client.getNickname(), client.getUsername(), channelName, arguments[2], kickReason);

    channel->kickClient(&client, clientToKick, kickReason);
    return 1; // Success
}