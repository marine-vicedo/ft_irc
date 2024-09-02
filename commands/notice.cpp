# include "../commands.hpp"

int sendNoticeToChannel(IRCServer &server, Client &client, const std::string &receiver, const std::string &message) {
    Channel *channel = server.getChannelByName(receiver.substr(1));
    if (channel && channel->isUser(&client)) {
        channel->sendToChannel(NOTICE(client.getNickname(), client.getHostname(), receiver, message), &client);
        return 1;
    } else if (!channel) {
        client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), receiver));
        return 0;
    }
    return 1; // Pour les canaux, la vérification de l'appartenance au canal n'est pas nécessaire pour NOTICE.
}

int sendNoticeToClient(IRCServer &server, Client &client, const std::string &receiver, const std::string &message) {
    Client *receiverClient = server.getClientByNickname(receiver);
    if (receiverClient) {
        receiverClient->msg(NOTICE(client.getNickname(), client.getHostname(), receiver, message));
        return 1;
    } else {
        client.msg(ERR_NOSUCHNICK(client.getNickname(), receiver));
        return 0;
    }
}
int sendMessageToReceiver(IRCServer &server, Client &client, const std::string &receiver, const std::string &message) {
    if (receiver[0] == '#') {
        std::cout << "msg to receiver OK" << std::endl;
        return sendNoticeToChannel(server, client, receiver, message);
    } else {
        return sendNoticeToClient(server, client, receiver, message);
    }
}
std::string buildMessageFromInput(const std::vector<std::string> &input) {
    std::string message;
    for (size_t i = 2; i < input.size(); ++i) {
        message += input[i] + " ";
    }
    if (!message.empty() && message[0] == ':') {
        message = message.substr(1);
    }
    return message;
}

int notice(IRCServer &server, Client &client, std::vector<std::string> &arguments) {
    // Vérifie si le client est authentifié ou enregistré.
    if (!client.getAuth() && !client.getRegistered()) {
        client.msg(ERR_NOTREGISTERED(client.getNickname(), "NOTICE"));
        return 0;
    }

    // Vérifie si suffisamment de paramètres ont été fournis.
    if (arguments.size() < 3) {
        client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
        return 0;
    }

    // Construit le message à partir des paramètres d'entrée.
    std::string receiver = arguments[1];
    std::string message = buildMessageFromInput(arguments);

    // Envoie le message au destinataire, qui peut être un canal ou un client.
    return sendMessageToReceiver(server, client, receiver, message);
}



