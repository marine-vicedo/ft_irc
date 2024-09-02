#include "../commands.hpp"

int pass(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	
	if (client.getRegistered()) {
		client.msg(ERR_ALREADYREGISTRED(client.getNickname()));
		return 0;
	}

	if (arguments.size() < 2 || server.getPassword() != arguments[1]) {
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(),arguments[0]));
		return 0;
	}
	if (server.getPassword() == arguments[1])
		client.setAuth(true);
	//C'est un indicateur booléen pour représenter si le client est authentifié.
	return 1;
}