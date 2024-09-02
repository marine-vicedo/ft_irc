#include "../commands.hpp"

int die(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	(void)server;
	(void)arguments;
	
	if (!client.getAuth() && !client.getRegistered())
	{
		// Client already authenticate
		client.msg(ERR_NOTREGISTERED(client.getNickname(), "DIE"));
		return 0;
	}

	// Only allow IRC operators to use this command.
	if (!client.getOp()) 
	{
		client.msg(ERR_NOPRIVILEGES(client.getNickname()));
		return 0;
	}

	g_shutdown = true;

	return 1;
}