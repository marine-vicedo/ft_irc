#include "../commands.hpp"

// input[0] = invite command
// input[1] = nickname of user to invite
// input[2] = channel name
int invite(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	if (arguments.size() < 3)
	{
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
		return (0);
	}

	if (!client.getAuth() && !client.getRegistered())
	{
		client.msg(ERR_NOTREGISTERED(client.getNickname(), "INVITE"));
		return 0;
	}
	std::string channelName = arguments[2];
	if (channelName[0] == '#')
		channelName = arguments[2].substr(1);

	Channel *channel = server.getChannelByName(channelName);
	if (!channel)
	{
		// The channel doesn't exist.
		client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return (0);
	}
	// Check if client is part of the channel.
	if (!channel->isUser(&client))
	{
		// The client is not part of the specified channel.
		client.msg(ERR_NOTONCHANNEL(client.getNickname(), channelName));
		return (0);
	}

	Client *clientToInvite = server.getClientByNickname(arguments[1]);
	if (!clientToInvite)
	{
		// The user to invite doesn't exist.
		client.msg(ERR_NOSUCHNICK(client.getNickname(), arguments[1]));
		return (0);
	}

	// Check if client to invite is already part of the channel.
	if (channel->isUser(clientToInvite))
	{
		client.msg(ERR_USERONCHANNEL(client.getNickname(), channelName, arguments[1]));
		return (0);
	}

	client.msg(RPL_INVITING(client.getNickname(), clientToInvite->getNickname(), channelName));
	clientToInvite->msg(INVITE(client.getNickname(), client.getUsername(), clientToInvite->getNickname(), channelName));
	
	// Invite the user to the channel.
	channel->addInvite(clientToInvite);

	return (1);
}