#include "../commands.hpp"

static bool invalidChars(std::string nickname)
{
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-')
			return true;
	}
	return false;
}

int nick(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	
	if (arguments.size() < 2)
	{
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
		return 0;
	}

	if (arguments[1].empty())
	{
		client.msg(ERR_NONICKNAMEGIVEN(client.getNickname()));
		return 0;
	}

	if (invalidChars(arguments[1]))
	{
		client.msg(ERR_ERRONEUSNICKNAME(client.getNickname(), arguments[1]));
		return 0;
	}
	std::string newNickname = arguments[1];//check la validite du nickname

	// If user try to change nickname to one that already exist
	if (server.getClientByNickname(newNickname) && !client.getNickname().empty())
	{
			client.msg(ERR_NICKNAMEINUSE(newNickname));
			return 0;
	}
	while (server.getClientByNickname(newNickname))
		newNickname += "_";

	// Update the client's nickname
	client.setNickname(newNickname);

	// Notify the client that the nickname change was successful
	//message "You're now known as .."
	client.msg(NICK(client.getNickname(), client.getHostname(), newNickname));

	if (!client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty())
		client.setRegistered(true);

	if (client.getRegistered() && client.getAuth())
	{
		client.msg(RPL_WELCOME(client.getNickname()));
	}
	return 1;
}

// Check if the nickname contains invalid characters.
