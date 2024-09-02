#include "../commands.hpp"

int ping(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	(void)server;
	if (arguments.size() < 2) {
		client.msg(ERR_NOORIGIN(client.getNickname()));
		return 0;
	}
	// Vérifie si le serveur spécifié dans la commande PING est correct.
	if (arguments[1] != SERVER_NAME) {
		client.msg(ERR_NOSUCHSERVER(client.getNickname()));
		return 0;
	}
	// Répond au PING avec un PONG.
	client.msg(PONG(client.getNickname()));
	return 1;
}