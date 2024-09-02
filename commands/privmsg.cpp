#include "../commands.hpp"


// arguments[0] = /msg (et non /PRIVMSG)
// arguments[1] = receiver (client / channel)
// arguments[...] = message to send
                                /*commentaire pour marine*/
//erreur das ta fct privmsg que tu envoi un msg NULL de coup c normale que les client de chanelle il recoive rien j'ai change que sa
//if (!msgToSend.empty())
			//channel->sendToChannel(PRIVMSG(client.getNickname(), client.getUsername(), arguments[1], msgToSend),&client); 
//j'ai rajouter une modification au fct sendtochannel car le client qui envoie msg il le recoie deux fois et j'ai changer dans classe serveur partie chanelle 
// commands.cpp (ou le fichier où se trouve votre fonction privmsg)
int privmsg(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
    if (arguments.size() < 3)
    {
        client.msg(ERR_NEEDMOREPARAMS(client.getNickname(), arguments[0]));
        return 0;
    }

    if (!client.getRegistered() && !client.getAuth())
    {
        client.msg(ERR_NOTREGISTERED(client.getNickname(), arguments[0]));
        return 0;
    }

    std::string msgToSend;
    for (size_t i = 2; i < arguments.size(); i++)
        msgToSend += arguments[i] + " ";

    if (msgToSend[0] == ':')
        msgToSend = msgToSend.substr(1);

    if (arguments[1][0] == '#') 
    {
        Channel *channel = server.getChannelByName(arguments[1].substr(1));
        if (!channel)
        {
            client.msg(ERR_NOSUCHCHANNEL(client.getNickname(), arguments[1]));
            return 0;
        }
        if (!channel->isUser(&client))
        {
            client.msg(ERR_NOTONCHANNEL(client.getNickname(), arguments[1]));
            return 0;
        }
        if (!msgToSend.empty())
            channel->sendToChannel(PRIVMSG(client.getNickname(), client.getUsername(), arguments[1], msgToSend), &client);
    }
    else 
    {
       	if (arguments[1] == "bot")
        {
            //arguments.erase(arguments.begin());

            bot(server, client, arguments);
            return 1; 
        }
	    Client *receiver = server.getClientByNickname(arguments[1]);
        if (!receiver)
        {
            client.msg(ERR_NOSUCHNICK(client.getNickname(), arguments[1]));
            return 0;
        }

        receiver->msg(PRIVMSG(client.getNickname(), client.getUsername(), arguments[1], msgToSend));
        return 1;
    }
    return 1;
}

