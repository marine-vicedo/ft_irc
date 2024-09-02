#include "../commands.hpp"
#include <string>

// #define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVER_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")
// #define	SERVER_NAME "Ircserver"
// #define ERR_NICKNAMEINUSE(nickname) (std::string(":") + SERVER_NAME + " 433 " + nickname + " :Nickname is already in use\r\n")
// #define RPL_WELCOME(nickname) (std::string(":") + SERVER_NAME + " 001 " + nickname + " :Welcome to the " + SERVER_NAME + " Server " + nickname + "\r\n")
// #define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + SERVER_NAME + " " + command + " ")
// #define ERR_NOTREGISTERED(nickname, command) (std::string(":") + SERVER_NAME + " 451 " + nickname + " " + command + " :You have not registered" + "\r\n")


//Mode + i et Mode + o au niveau du user
//mode <user> + i : passe le user en mode invisible, pour rester discret
//mode <user> + o : donne a un user le statut de channel operator : peut inviter des users à rejoindre le salon, bannir des users du salon et de modifier les modes du salon.

int userMode(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	(void)server;
	std::string output = "";

	bool add = true;
	for (size_t i = 0; i < arguments[2].size(); i++)
	{
		char mode = arguments[2][i];
		if (mode == '+')
			add = true;
		else if (mode == '-')
			add = false;
		else
		{
			if (mode == 'i')
				client.setInvisible(add);
			// else if (mode == 'w')
			// 	client.setWallops(add);
			else
			{
				client.msg(ERR_UMODEUNKNOWNFLAG(client.getNickname()));
				return (0);
			}
			if (add)
			   output += "+";
			else
			   output += "-";
		   output += mode;
		}
	}
	client.msg(MODE_USER(client.getNickname(), client.getUsername(), client.getNickname(),output));

	return (1);
}

void handleInviteOnlyMode(bool add, Channel* channel)
{
	channel->setInviteOnly(add);
	return;
}

void handleTopicRestrictedMode(bool add, Channel* channel)
{
	//std::cout << "topic restricted" << add << std::endl;
	channel->setTopicRestricted(add);
	return;
}

void handleChannelPassword(bool add, Channel* channel, Client& client, std::vector<std::string>& arguments)
{
	std::string empty = "";
	if (add)
	{
		if (arguments.size() < 4)
		{
			return(client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE")));
		}
		channel->setChannelPassword(arguments[3]);
	}
	else
		channel->setChannelPassword(empty);
	return;
}

void handleOperator(bool add, IRCServer &server, Channel *channel, Client& client, std::vector<std::string>& arguments)
{
	if (arguments.size() < 4)
		return (client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE")));

	Client *op = server.getClientByNickname(arguments[3]);
	if (!op)
		return (client.msg(ERR_USERNOTINCHANNEL(client.getNickname(), arguments[1].substr(1), arguments[3])));
	if (add)
		channel->addOperator(op);
	else
		channel->removeOperator(op);
}

void handleUserLimit(bool add, Channel* channel, Client& client, std::vector<std::string>& arguments)
{
	//std::cout << "handle user limit" << std::endl;
	if (add)
	{
		//std::cout << "apres add" << std::endl;
		if (arguments.size() < 4)
			return(client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0])));
		channel->setMaxUsers(std::atoi(arguments[3].c_str()));//set the max users
	}
	else
		channel->setMaxUsers(0);//remove the max users
	return;
}

// MODE #channel +t : change le topic

// MODE #channel +o user = un opérateur du canal peut donner le statut d'opérateur à un autre utilisateur.
// MODE #channel +o = possible de se donner le statut d'opérateur sur un canal si vous avez les droits nécessaires.

//MODE #salon +k motdepasse = un mdp est requis pour rejoindre le channel

int channelMode(IRCServer &server, Client &client, std::vector<std::string> &arguments, Channel *channel)
{
	bool add = true;
	std::string output = "";

	// if (arguments.size() < 3)
	// 	return 0;

	for (size_t i = 0; i < arguments[2].size(); i++)
	{
		char mode = arguments[2][i];

		if (mode == '+')
			add = true;
		else if (mode == '-')
			add = false;

		if (mode == 'i')
			handleInviteOnlyMode(add, channel);
		else if (mode == 't')
			handleTopicRestrictedMode(add, channel);
		else if (mode == 'k')
			handleChannelPassword(add, channel, client, arguments);
		else if (mode == 'o')
		{
			handleOperator(add, server, channel, client, arguments);
			names(server,client,arguments);
		}
		else if (mode == 'l')
		{ 
			handleUserLimit(add, channel, client, arguments);
		}
		else
		{
			client.msg(ERR_UMODEUNKNOWNFLAG(client.getNickname()));
			continue;
		}
		
		if (add)
			output += "+";
		else
			output += "-";
		output += mode;
	}
	client.msg(MODE_CHANNEL(client.getNickname(), client.getUsername(), arguments[1].substr(1), output));
	return 1;
}




int mode(IRCServer &server, Client &client, std::vector<std::string> &arguments)
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

	if (arguments[1][0] == '#')
	{
		//verifier que le channel existe
		Channel *channel = server.getChannelByName(arguments[1].substr(1));
		if (!channel)
		{
			client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
			return (0);
		}
		if (!channel->isOperator(&client))
		{
			//std::cout << "operator" << std::endl;
			client.msg(ERR_CHANOPRIVSNEEDED(client.getNickname(), arguments[1]));
			return (0);
		}
		//verifier que le client est le channel op pour executer cette commande
		return (channelMode(server, client, arguments, channel));
	}
	else if (arguments[1] == client.getNickname())
		return (userMode(server, client, arguments));
	else
		client.msg(ERR_USERSDONTMATCH(client.getNickname()));
	return (0);
}

