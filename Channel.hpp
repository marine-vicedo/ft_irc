#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <iterator>
# include <sstream>
# include <fstream>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>
# include <algorithm>

# include "Ircserver.hpp"
# include "Client.hpp"



class Client;
class IRCServer;

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		IRCServer				&_server;
		Client					*_admin;
		std::string				_channelPassword;
		std::map<int,Client *>	_users;// liste des users dans le channel
		std::vector<Client *>	_operators;// liste des opérateurs dans le channel
		std::vector<Client *>	_inviteList;// liste des invites
		bool					_inviteOnly;
		bool					_topicRestricted;
		size_t					_maxUsers;// pour limit
		
	public:
		Channel(IRCServer &server,std::string name, Client *client);
		~Channel();

		std::map<int, Client*>getClients() const { return _users; }
		std::vector<Client *>getOperators() const { return _operators; }
		Client 					*getadmin() { return _admin; };

		std::string getName();
		std::string getTopic();
		int getMaxUsers();
		bool getInviteOnly();
		bool getTopicRestricted();

		void setName(std::string name);
		void setInviteOnly(bool inviteOnly);
		void setMaxUsers(int maxUsers);
		void setChannelPassword(std::string &password);
		void setTopic(std::string topic);
		void setTopicRestricted(bool topicRestricted);

		void changeTopic(std::string &topic);
		void isValidChannelName(std::string &name);
		std::string getChannelPassword();
		bool isValidChannelPassword(std::string &password);

		void addUser(Client *user);
		void removeUser(Client *user);
		void addOperator(Client* user);
		void removeOperator(Client *user);
		
		void addInvite(Client *user);
		void removeInvite(Client *user);
		int isInvited(Client *client);

		bool validateJoin(Client *client, const std::vector<std::string> &input);
		int isUser(Client *user);
		int isOperator(Client *user);
		void sendToChannel(const std::string &message,Client *sender);
		Client* getClientByNickname(const std::string &nickname);  
		void kickClient(Client* kicker, Client* clientToKick, const std::string& reason);

};
#endif