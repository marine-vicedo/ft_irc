# include "../commands.hpp"

int user(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
	(void)server;
	if (arguments.size() < 5)
	{
		// Envoie un message d'erreur si la commande USER n'a pas assez de paramètres.
		client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
		return 0;
	}
	if (client.getRegistered())
	{
	// Envoie un message d'erreur si le client est déjà enregistré.
		client.msg(ERR_ALREADYREGISTRED(client.getNickname()));
		return 0;
	}

	// Si toutes les conditions sont satisfaites, enregistrez les informations de l'utilisateur.
	client.setUsername(arguments[1]);
	client.setRealname(arguments[4]);

	// Vérifie si l'utilisateur est authentifié ).
	if (!client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty())
		client.setRegistered(true);
	if (client.getRegistered() && client.getAuth())
	{
		// Envoyer un message de bienvenue ou effectuer d'autres actions nécessaires.
		client.msg(RPL_WELCOME(client.getNickname()));
		return 1;
	}
	return 1;
}
