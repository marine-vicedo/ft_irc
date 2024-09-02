#include "../commands.hpp"

// INFO command
// input[0] = INFO command
// input[1] = server name
int info(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	(void)server;
	// Check if the necessary arguments are provided
	if (arguments.size() >= 2 && arguments[1] != SERVER_NAME)
	{
		client.msg(ERR_NOSUCHSERVER(client.getNickname()));
		return 0;
	}

	client.msg(RPL_INFO(client.getNickname()));
	client.msg(RPL_ENDOFINFO(client.getNickname()));
	return 1;
}