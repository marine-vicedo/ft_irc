#include "../commands.hpp"

//PART:La commande PART permet à un utilisateur de quitter un salon IRC
// input[0] = part command
// input[1] = channel
// input[2] = message (optional)

// std::vector<std::string> splitChan(std::string channelsInput, char delim)
// {
// 	size_t start = 0;
// 	size_t end = 0;
// 	std::vector<std::string> vchan;

// 	while ((start = channelsInput.find_first_not_of(delim, end)) != std::string::npos)
// 	{
// 		end = channelsInput.find(delim, start);
// 		vchan.push_back(channelsInput.substr(start, end - start));
// 	}

// 	for (size_t i = 0; i != vchan.size(); i++)
// 	{
// 		std::cout << vchan[i];
// 	}
// 	return (vchan);
// }

int part(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2)
	{
		//std::cout << "arg pbm" << std::endl;
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
		return (0);
	}
	if (!client.getRegistered() && !client.getAuth())
	{
		//std::cout << "client not registered" << std::endl;
		client.msg(ERR_NOTREGISTERED(client.getNickname(), arguments[0]));
		return (0);
	}

	//std::vector<std::string> channels = splitChan(arguments[1], ',');
	if (arguments[1].empty())
	{
		//std::cout << "no channel" << std::endl;
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
		return (0);
	}
	else {

	Channel *channelToLeave = server.getChannelByName(arguments[1].substr(1));
	if (!channelToLeave)
	{
		//std::cout << "no channel" << std::endl;
		client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
		return (0);
	}
	if (!channelToLeave->isUser(&client))
	{
		//std::cout << "user not part of the chan" << std::endl;
		// The client is not part of the specified channel.
		client.msg(ERR_NOTONCHANNEL(client.getNickname(), arguments[1]));
		return (0);
	}
	std::string msg = "";
	for (size_t i = 2; i < arguments.size(); i++)
		msg = arguments[i] + " ";

	//rajouter un msg pour dire tchao
	channelToLeave->sendToChannel(RPL_PART(user_id(client.getNickname(),
		client.getUsername()), channelToLeave->getName(), msg), &client);

	client.quitChannel(channelToLeave);
	return (1);
	}

}