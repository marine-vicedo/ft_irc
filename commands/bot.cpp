#include "../commands.hpp"
#include <ctime>
#define CLIENT_IDI(nickname, username, command) getClientId(nickname, username, command)

std::string getClientId(const std::string& nickname, const std::string& username, const std::string& command)
{
    std::stringstream clientId;
    clientId << ":" << nickname << "!~" << username << "@" << SERVER_NAME << " " << command << " ";
    return clientId.str();
}
#define bot_PRIVMSG(nickname, username, dest, msg) (CLIENT_IDI(nickname, username, "PRIVMSG") + dest + " :" + msg + "\r\n")

void bot(IRCServer &server, Client &client, std::vector<std::string> &arguments)
{
    (void)server;
    
    if (arguments.size() < 3)
    {
        client.msg("Bot response: Please provide a command for the bot.");
        return;
    }

    std::string botCommand = arguments[2];

    if (botCommand == ":info")
    {
        client.msg(bot_PRIVMSG("Bot", "botuser", client.getNickname(), "I am a simple IRC bot."));
    }
    else if (botCommand == ":joke")
    {
        client.msg(bot_PRIVMSG("Bot", "botuser", client.getNickname(), "Why don't scientists trust atoms? Because they make up everything!"));
    }
    else if (botCommand == ":time")
    {
        // Obtenir l'heure actuelle
        std::time_t timeNow = std::time(NULL);
        std::string timeStr = std::ctime(&timeNow);
        timeStr.erase(timeStr.length() - 1);

        client.msg(bot_PRIVMSG("Bot", "botuser", client.getNickname(), "Current time is " + timeStr));
    }
    else if (botCommand == ":quote")
    {
        // Ajouter une citation aléatoire
        std::vector<std::string> quotes;
        quotes.push_back("To be or not to be, that is the question.");
        quotes.push_back("The only limit is your imagination.");
        quotes.push_back("Stay hungry, stay foolish.");

        srand(static_cast<unsigned int>(time(NULL)));
        int randomIndex = rand() % quotes.size();
        client.msg(bot_PRIVMSG("Bot", "botuser", client.getNickname(), "Quote of the day - " + quotes[randomIndex]));
    }
    else
    {
        client.msg(bot_PRIVMSG("Bot", "botuser", client.getNickname(), "Unknown command. Supported commands: info, joke, time, quote"));
    }
}
