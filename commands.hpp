#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Client.hpp"
# include "Ircserver.hpp"
# include "Channel.hpp"
# include "define.hpp"
# include "globals.hpp"
# include <vector>

class IRCServer;
class Client;
class Channel;

struct cmd_infos
{
	std::string cmd;
	
};

int cap(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int die(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int info(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int quit(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int invite(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int join(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int kick(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int mode(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int names(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int nick(IRCServer &server, Client &client, std::vector<std::string> &arguments);
//int notice(Server &server, Client &client, std::vector<std::string> &arguments);
int oper(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int part(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int pass(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int ping(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int privmsg(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int topic(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int user(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int wallops(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int whois(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int who(IRCServer &server, Client &client, std::vector<std::string> &arguments);
int notice(IRCServer &server, Client &client, std::vector<std::string> &arguments);
void bot(IRCServer &server, Client &client, std::vector<std::string> &arguments);
#endif