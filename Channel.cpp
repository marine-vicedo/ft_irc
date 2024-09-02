#include "Channel.hpp"
#include "Ircserver.hpp"
#include <string>
#include <cctype>

Channel::Channel(IRCServer &server,std::string name, Client *client)
: _server(server), _admin(client),_topicRestricted(false)
{
	try{
		isValidChannelName(name);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return ;
	}
	
	this->_name = name;
	this->_topic = "";
	this->_channelPassword = "";
	this->_maxUsers = 0;
	this->_inviteOnly = false;
	this->_users.clear();
	this->_operators.clear();
	this->_inviteList.clear();
	//this->_topicRestricted = true;
	addUser(client);// le client qui cree le channel est dans le channel
	addOperator(client);// le client qui cree le channel est operator
	
}

Channel::~Channel(){
	/*for (std::map<int, Client*>::iterator it = _users.begin(); it != _users.end(); ++it)
			it->second->quitChannel(this);*/
};

std::string Channel::getName(){return (this->_name);}

std::string Channel::getTopic(){return (this->_topic);}

void Channel::setName(std::string name){ this->_name = name;}

void Channel::setTopic(std::string topic){this->_topic = topic;}

void Channel::setInviteOnly(bool inviteOnly){this->_inviteOnly = inviteOnly;}

bool Channel::getInviteOnly(){return (this->_inviteOnly);}

int Channel::getMaxUsers(){return (this->_maxUsers);}

void Channel::setMaxUsers(int maxUsers){this->_maxUsers = maxUsers;}

void Channel::setChannelPassword(std::string &password){this->_channelPassword = password;}

std::string Channel::getChannelPassword(){return (this->_channelPassword);}

bool Channel::getTopicRestricted(){return (this->_topicRestricted);}

void Channel::setTopicRestricted(bool topicRestricted){this->_topicRestricted = topicRestricted;}


void Channel::isValidChannelName(std::string &channelName)
{
	if (channelName.find(' ') != std::string::npos && channelName.find(',') != std::string::npos && channelName.find('\07') != std::string::npos )
		throw std::invalid_argument("Channel name contains forbidden characters");
	if(channelName.size() > 50)
		throw std::invalid_argument("Channel name too long");
	if(channelName.size() < 2)
		throw std::invalid_argument("Channel name too small");
	// if(channelName[0] != '#' && channelName[0] != '&')
	// 	throw std::invalid_argument("Channel name must start with # or &");
	// for (size_t i = 1; i < channelName.size(); i++)
	// {
	// 	if (!std::isalnum(channelName[i]) && channelName[i] != '-' && channelName[i] != '_')
	// 		throw std::invalid_argument("Channel name contains forbidden characters");
	// }
}

bool Channel::isValidChannelPassword(std::string &password)
{
	if(this->_channelPassword == password)
		return (true);
	else
		throw std::invalid_argument("Invalid channel password");
}
/*********************USER*******************/
int Channel::isUser(Client *user)
{
	if (!user)
		return 0;

	std::map<int, Client*>::iterator it;
	for (it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second == user)
			return 1;
	}
	return 0;
}

void Channel::addUser(Client *user)
{
	if (!user) 
		return;
	if (_maxUsers > 0 && _users.size() >= _maxUsers)
	{
		user->msg("Channel is full");
		return;
	}
	int fd =user->getFd();
	if (_users.find(fd) != _users.end())
	{
		//user->msg("User is already in channel");
		return;
	}
	_users[fd] = user;
}

void Channel::removeUser(Client *user)
{
	if (!user)
		return;

	std::map<int, Client*>::iterator it = _users.begin();
	for (; it != _users.end(); ++it)
	{
		if (it->second == user)
			break;
	}
	if (it != _users.end())
	{
		_users.erase(it);
		if (user == _admin)
			_admin = 0;
		removeInvite(user);
		removeOperator(user); 
		if (_operators.empty() && !_users.empty())
			addOperator(_users.begin()->second);
		if (_users.empty() && _operators.empty())
		   _server.removeChannel(this); 
	}
}
/********************operator************************/
void Channel::addOperator(Client* user)
{
	// Vérifier si l'utilisateur est présent dans la map _users
	if (_users.find(user->getFd()) != _users.end())
	{
		// Vérifier si l'utilisateur n'est pas déjà dans le vecteur _operators
		bool userFound = false;
		for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
		{
			if (*it == user)
			{
				userFound = true;
				break;
			}
		}
		if (!userFound)
		{
			_operators.push_back(user);
			//user->setOp(true);
		}
	}
}

void Channel::removeOperator(Client* user)
{
	// Vérifier si le client est dans le vecteur _operators
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it == user)
		{
			_operators.erase(it);
			break;
		}
	}
}

int Channel::isOperator(Client *user)
{
	if (!user)
	{
		return 0;
	}
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it == user)
		{
			return 1;
		}
	}
	return 0;
}

/*************************Nickname client************************/
Client* Channel::getClientByNickname(const std::string &nickname)
{
	std::map<int, Client*>::iterator it;
	for (it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return it->second;
	}
	return NULL;
}

void Channel::addInvite(Client *user)
{
	bool userFound = false;
	for (std::vector<Client*>::iterator it = _inviteList.begin(); it != _inviteList.end(); ++it)
	{
		if (*it == user)
		{
			userFound = true;
			break;
		}
	}

	if (!userFound)
	{
		_inviteList.push_back(user);
	}
}

void Channel::removeInvite(Client *user)
{
	// Vérifier si le client est dans le vecteur _operators
	for (std::vector<Client*>::iterator it = _inviteList.begin(); it != _inviteList.end(); ++it)
	{
		if (*it == user)
		{
			_inviteList.erase(it);
			break;
		}
	}
}
int Channel::isInvited(Client *user)
{
	if (!user)
		return 0;
	std::vector<Client*>::iterator invitedIt;
	for (invitedIt = _inviteList.begin(); invitedIt != _inviteList.end(); ++invitedIt)
	{
		if (*invitedIt == user)
			return 1;
	}
	return 0;
}

void Channel::sendToChannel(const std::string &message,Client *sender)
{ 
	for (std::map<int, Client*>::iterator it = _users.begin(); it != _users.end(); ++it)
	if (it->second != sender)
            it->second->msg(message);
}

/*********************pour la commande join***************************/
bool Channel::validateJoin(Client *client, const std::vector<std::string> &input)
{
	if (!client || input.empty())
		return false;

	if (!client->getAuth() && !client->getRegistered())
	{
		client->msg(ERR_NOTREGISTERED(client->getNickname(), "JOIN"));
		return false;
	}

	if (input.size() < 2)
	{
		client->msg(ERR_NEEDMOREPARAMS(client->getNickname(), input[0]));
		return false;
	}

	std::string channelName = input[1].substr(1);

	if (!getChannelPassword().empty() && (input.size() < 3 || getChannelPassword() != input[2]))
	{
		client->msg(ERR_BADCHANNELKEY(client->getNickname(), getName()));
		return false;
	}

	if (getInviteOnly() && !isInvited(client))
	{
		client->msg(ERR_INVITEONLYCHAN(client->getNickname(), getName()));
		return false;
	}

	if (getMaxUsers() && getClients().size() >= static_cast<std::map<int, Client*>::size_type>(getMaxUsers()))
	{
		client->msg(ERR_CHANNELISFULL(client->getNickname(), getName()));
		return false;
	}

	return true;
}
void Channel::kickClient(Client* kicker, Client* clientToKick, const std::string& reason)
{
    if (!kicker || !clientToKick) {
        // Gestion des cas où les pointeurs sont nuls
        return;
    }

    std::string kickMessage = KICK(kicker->getNickname(), kicker->getUsername(), getName(), clientToKick->getNickname(), reason);

    clientToKick->msg(kickMessage);

    sendToChannel(kickMessage, clientToKick);

    removeUser(clientToKick);
}
