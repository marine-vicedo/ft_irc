#include "../commands.hpp"

int topic(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
    // Vérifie si le client est authentifié ou enregistré
    if (!client.getAuth() && !client.getRegistered())
    {
        client.msg(ERR_NOTREGISTERED(client.getNickname(), "TOPIC"));
        return 0;
    }

    // Vérifie si le nombre d'arguments est suffisant
    if (arguments.size() < 2)
    {
        client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
        return 0;
    }

    // Extrait le nom du canal à partir du deuxième argument
    std::string channelName = arguments[1].substr(1);
    Channel *channel = server.getChannelByName(channelName);

    // Vérifie si le canal existe
    if (!channel)
    {
        client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return 0;
    }

    // Vérifie si le client est membre du canal
    if (!channel->isUser(&client))
    {
        client.msg(ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return 0;
    }

    // Si aucun sujet n'est fourni, renvoie le sujet actuel
    if (arguments.size() == 2)
    {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty())
            client.msg(RPL_NOTOPIC(client.getNickname(), channel->getName()));
        else
            client.msg(RPL_TOPIC(client.getNickname(), channel->getName(), currentTopic));
        return 1;
    }
    else // Un nouveau sujet est fourni. Définit le nouveau sujet.
    {
        // Vérifie si le changement de sujet est autorisé (topicRestricted)
		//std::cout<<"topic dans fct topc"<<channel->getTopicRestricted()<<std::endl;
        if (channel->getTopicRestricted())
        {//std::cout<<"topic dans fct topc 1"<<channel->getTopicRestricted()<<std::endl;
            // Seuls les opérateurs peuvent changer le sujet.
            if (!channel->isOperator(&client))
            { 
				std::cout<<"topic dans fct topc 2"<<channel->getTopicRestricted()<<std::endl;
                client.msg(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
                return 0;
            }
        }
		else if (!channel->getTopicRestricted())
		{

        	client.msg(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
			return 0;
		}
		//std::cout<<"topic dans fct topc 4"<<channel->getTopicRestricted()<<std::endl;
        // Construit le nouveau sujet à partir des arguments
        std::string topic = "";
        for (int i = 2; i < (int)arguments.size(); i++)
            topic += arguments[i] + " ";

        // Supprime le préfixe ':' s'il est présent
        if (topic[0] == ':')
            topic = topic.substr(1);

        // Définit le nouveau sujet
        channel->setTopic(topic);

        // Notifie le client que le sujet a été changé avec succès.
        channel->sendToChannel(TOPIC(client.getNickname(), client.getUsername(), channel->getName(), channel->getTopic()), NULL);
    }

    return 1;
}

