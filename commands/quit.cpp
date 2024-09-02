#include "../commands.hpp"

// Command to quit the irc
// input[0] = quit command
// input[1] = message why quit (optional)
int quit(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
    std::vector<Channel *> channels = client.getChannels();

    // Définir un message de départ par défaut.
    std::string quitMessage = "Client quit";
    if (arguments.size() > 1) {
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
        quitMessage = arguments[1];
    }

    // Notifier les autres clients dans les canaux dont ce client est membre.
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = *it;
        // Envoyer le message de départ à chaque canal.
        channel->sendToChannel(QUIT(client.getNickname(), client.getUsername(), quitMessage), &client);
    }

    // Déconnecter le client.
    server.removeClient(client.getFd());

    return 1; // Succès
}
