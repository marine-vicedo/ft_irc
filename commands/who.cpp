#include "../commands.hpp"

int who(IRCServer &server, Client &client, std::vector<std::string> &arguments) {
    if (arguments.size() >= 2) {
        std::string channelName = arguments[1];
        Channel *channel = server.getChannelByName(channelName.substr(1));

        if (channel) {
            // Afficher le nom du canal
            //std::cout << "Channel Name: " << channelName << std::endl;
			std::vector<Client*> operatorsList = channel->getOperators();

            // Afficher les informations sur chaque client dans le canal
            std::map<int, Client*> clientsMap = channel->getClients();
            //std::cout << "Number of Clients in Channel: " << clientsMap.size() << std::endl;

            // Envoyer les messages RPL_WHOREPLY et RPL_ENDOFWHO à l'utilisateur
            for (std::map<int, Client*>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it) {
    		Client* clientPtr = it->second;
    		std::string flags = "H"; // Par défaut, suppose que tous les clients sont dans le canal
    		if (clientPtr == channel->getadmin()) 
				flags += "@";
			else if (std::find(operatorsList.begin(), operatorsList.end(), it->second) != operatorsList.end()) 
                    flags += "@";
   
    		std::string whoReply = RPL_WHOREPLY(client.getNickname(), channelName.substr(1), clientPtr->getUsername(),
                                        clientPtr->getHostname(), server.getServerName(),
                                        clientPtr->getNickname(), flags, "0 " + clientPtr->getRealname());
    		client.msg(whoReply);
			}


std::string endOfWho = RPL_ENDOFWHO(client.getNickname(), channelName.substr(1));
client.msg(endOfWho);


            client.msg(RPL_ENDOFWHO(client.getNickname(), channelName));
        } else {
            // Afficher un message si le canal n'existe pas
            //std::cout << "Channel does not exist: " << channelName << std::endl;
            client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        }
    } else {
        // Afficher un message si la commande n'a pas assez d'arguments
        std::cout << "Not enough arguments for WHO command" << std::endl;
    }

    // Afficher un message indiquant la fin de la commande WHO
    std::cout << "End of WHO command" << std::endl;

    return 1;
}
