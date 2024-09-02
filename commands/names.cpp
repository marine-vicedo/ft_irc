#include "../commands.hpp"

//utilisée pour obtenir la liste des utilisateurs présents dans un canal
//spécifié sur le serveur IRC. Je vais expliquer le code en détail.
// std::map<int, Client *> vectorToMap(const std::vector<Client *> &clients)
// {
// 	std::map<int, Client *> clientsMap;
// 	for (size_t i = 0; i < clients.size(); ++i)
// 	{
// 		// Supposons que getClientFD est une fonction de la classe Client qui renvoie le FD
// 		int fd = clients[i]->getFd();
// 		clientsMap[fd] = clients[i];
// 	}
// 	return clientsMap;
//}

void sendNamesReply(Client &client, const std::string &channelName, const std::string &clientsName) {
    client.msg(RPL_NAMREPLY(client.getNickname(), channelName, clientsName));
    client.msg(RPL_ENDOFNAMES(client.getNickname(), channelName));
}

int names(IRCServer &server, Client &client, std::vector<std::string> &arguments) {
    std::string channelName;
    std::string clientsName;

    if (arguments.size() >= 2) {
        channelName = arguments[1];
        Channel *channel = server.getChannelByName(channelName.substr(1));

        if (channel) {
            // Récupérer la liste mise à jour des opérateurs et des clients
            std::vector<Client*> operatorsList = channel->getOperators();
            std::map<int, Client*> clientsMap = channel->getClients();

            for (std::map<int, Client*>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it) {
                if (it->second == channel->getadmin())
                    clientsName += "@";
                else if (std::find(operatorsList.begin(), operatorsList.end(), it->second) != operatorsList.end()) {
                    clientsName += "@";
                }
                clientsName += it->second->getNickname() + " ";
            }
        }
    } else {
        // Récupérer la liste des clients sur le serveur
        std::map<int, Client*> clientsMap = server.getClients();
        for (std::map<int, Client*>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
            clientsName += it->second->getNickname() + " ";
    }

    // Appeler la fonction pour envoyer les messages RPL_NAMREPLY et RPL_ENDOFNAMES
    sendNamesReply(client, channelName, clientsName);

    return 1;
}

