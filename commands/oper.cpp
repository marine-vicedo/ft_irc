#include "../commands.hpp"
// La commande /oper permet de t'authentificier au près du serveur
// afin que ce dernier te donne le status de IRCOperator.
// input[0] = oper command
// input[1] = name
// input[2] = password


int oper(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	if (arguments.size() < 3)
	{
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
		return (0);
	}

	if (!client.getRegistered() && !client.getAuth())
	{
		client.msg(ERR_NOTREGISTERED(client.getNickname(), arguments[0]));
		return (0);
	}

	Client *clientToSetOp = server.getClientByNickname(arguments[1]);
	if (!clientToSetOp)
	{
		// Client not found.
		client.msg(ERR_NOSUCHNICK(client.getNickname(), arguments[1]));
		return 0;
	}

	// Check if the name and password combination is valid.
	if (server.getPassword() != arguments[2])
	{
		// The credentials are not valid.
		client.msg(ERR_PASSWDMISMATCH(client.getNickname()));
		return (0);
	}
	// The credentials are valid. Give the client operator privileges.
	clientToSetOp->msg(RPL_YOUROPER(clientToSetOp->getNickname()));
	clientToSetOp->setOp(true);

	return 1;

}