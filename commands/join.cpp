#include "../commands.hpp"

int join(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{

	if (arguments[1][0] != '#' && arguments[1][0] != '&')
	{
		client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
		return (0);
	}
	if (arguments.size() < 2)
	{
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return (0);
	}
	for(long unsigned int i = 1; i < arguments[1].size(); i++)
	{
		if (!isalnum(arguments[1][i]))
		{
			client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
			return (0);
		}
	}
	std::string channelName = arguments[1].substr(1);
	Channel *channel = server.getChannelByName(channelName);

	if (!channel)
	{
		channel = new Channel(server, channelName, &client);
		server.addChanell(channel);
	}
	else
	{
		if (!channel->validateJoin(&client, arguments))
			return 0;
	}

	client.joinChannel(channel);
	names(server, client, arguments);
	channel->sendToChannel(JOIN(client.getNickname(), client.getUsername(), channel->getName()),&client);
	channel->removeInvite(&client);
	return 1;
}