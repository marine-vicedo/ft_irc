#include "Ircserver.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "Client.hpp"
#include "commands.hpp"

void splitMessage(std::vector<std::string> &vcmds, std::string &msg)
{
	char delim = ' ';
	size_t start = 0;
	size_t end = 0;

	while ((start = msg.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = msg.find(delim, start);
		vcmds.push_back(msg.substr(start, end - start));
	}
	return;
}

int IRCServer:: parseCommand2(Client *client, std::string &message)
{
	std::stringstream msg(message);
	std::string cmd_line;
	while (std::getline(msg, cmd_line, '\n'))
	{
		if (cmd_line.end() - 1 != cmd_line.begin() && *(cmd_line.end() - 1) == '\n')
			cmd_line.erase(cmd_line.begin() + cmd_line.size() - 1, cmd_line.end());
		if (cmd_line.end() - 1 != cmd_line.begin() && *(cmd_line.end() - 1) == '\r')
			cmd_line.erase(cmd_line.begin() + cmd_line.size() - 1, cmd_line.end());
		
		if (cmd_line.empty())
			continue;

		std::vector<std::string> cmds_args;
		splitMessage(cmds_args, cmd_line);
		
		std::string command = cmds_args[0];
		
		std::transform(command.begin(), command.end(), command.begin(), ::toupper);

		std::map<std::string, CommandFunction>::iterator it = _commands.find(command);
	
		if (it != _commands.end())
		{
			it->second(*this, *client, cmds_args);
		}
		else
		{
			std::cout << "Invalid command received from client: " << command << std::endl;
		}
		if (command == "QUIT")
			return (0);
	}
	return (1);
}

// Traiter un nouveau message du client
void IRCServer::NewMsg(int client_fd)
{
	 std::map<int, Client*>::iterator it = this->clients.find(client_fd);
	
	if (it != this->clients.end())
	{
		int id = it->first;
		Client *client = it->second;
		
		char buffer[1024];
		ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
		
		if (bytes_read < 0)
		{
			std::cerr << "[Server] Failed to read from client" << std::endl;
			// removeClient(client_fd);
			return;
		}
		else if (bytes_read == 0)
		{

			return;
		}
		buffer[bytes_read] = '\0'; // Terminer la chaîne avec un caractère nul pour en faire une chaîne de caractères C

		std::cout << "[Client] Message received from client " <<  id << " : " << buffer << std::endl; // si affichage incoherent regarder ici
		std::string partialInput = client->getPartialInput() + std::string(buffer);
		client->setPartialInput(partialInput);
	
		if (!partialInput.empty() && partialInput[partialInput.size() - 1] == '\n')
		{
			if (parseCommand2(client, partialInput))
			client->setPartialInput("");
		}
	}
	else {
		std::cerr << "Client not found for the given file descriptor" << std::endl;
	}
	return;
}
/**************************clients********************************/
void IRCServer::addClient(int clientId, Client* client) {
	if (!client) 
		return;
	// Vérifie si le client existe déjà dans la map en utilisant son identifiant unique (clientId)
	if (clients.find(clientId) != clients.end()) {
		// Le client existe déjà
		return;
	}
	// Si le client n'existe pas encore, l'ajoute à la map avec son identifiant comme clé
	clients[clientId] = client;
}

void IRCServer::removeClient(int clientId) {
    std::map<int, Client*>::iterator it = clients.find(clientId);

    if (it != clients.end()) {
        // Supprimer le client des canaux auxquels il est connecté
        std::vector<Channel*> channels = it->second->getChannels();
        for (std::vector<Channel*>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt) {
            (*channelIt)->removeUser(it->second);
        }

        close(it->second->getFd());
        delete it->second;
        clients.erase(it);
    } else {
        std::cerr << "Client avec l'ID spécifié non trouvé." << std::endl;
    }
}

Client* IRCServer::getClientByFd(int fd) {
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getFd() == fd) {
			return it->second;
		}
	}
	return NULL;
}
Client* IRCServer::getClientByNickname(const std::string& nickname) {
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			return it->second;
		}
	}
	return NULL;
}

Client* IRCServer::getClientByUsername(const std::string& username) {
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getUsername() == username) {
			return it->second;
		}
	}
	return NULL;
}
void IRCServer::MsgForListClient(const std::string &message) {
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		sendMessage(it->first, message);
	}
}
/**************************chanell********************************/
void IRCServer::addChanell(Channel* channel)
{
    if (!channel)
        return;

    std::vector<Channel*>::iterator it;
    for (it = chanells.begin(); it != chanells.end(); ++it)
    {
        if (*it == channel)
        {
            return;
        }
    }
    chanells.push_back(channel);
}

void IRCServer::removeChannel(Channel* channel)
{
    // Utiliser un itérateur pour trouver l'élément dans le vecteur
    std::vector<Channel*>::iterator it = std::find(chanells.begin(), chanells.end(), channel);

    if (it != chanells.end())
    {
        delete *it;
        chanells.erase(it);
    }
}
Channel* IRCServer::getChannelByName(std::string name)
{
    // Utilisation d'une boucle pour parcourir le vecteur
    for (std::vector<Channel*>::iterator it = chanells.begin(); it != chanells.end(); ++it)
    {
        if ((*it)->getName() == name)  
        {
            return *it;
        }
    }
    return NULL;
}

const char* IRCServer::concatenate(const char* str1, const char* str2)
{
    // Alloue de la mémoire pour la nouvelle chaîne de caractères
    char* result = new char[strlen(str1) + strlen(str2) + 1];

    // Copie le contenu de la première chaîne de caractères
    strcpy(result, str1);

    // Concatène le contenu de la deuxième chaîne de caractères
    strcat(result, str2);

    return result;
}
void IRCServer::broadcastMessage(const char* message)
{
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) 
            it->second->msg(message);
}
/**********************************************************************/
int IRCServer::NewConnection(int epollFd, struct sockaddr_in &client_address, socklen_t &client_len){
	// Accepter une nouvelle connexion
	int client_fd = accept(serverSocket, (struct sockaddr*)&client_address, &client_len);
	//std::cout<<"numero" <<client_fd<<std::endl;
	if (client_fd < 0) {
		std::cerr << "Échec de l'acceptation du client." << std::endl;
		return 0;
	}
	// Récupérer le nom d'hôte du client
	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr*)&client_address, client_len, host, sizeof(host), NULL, 0, NI_NUMERICHOST) != 0) {
		std::cerr << "Impossible d'obtenir le nom d'hôte du client." << std::endl;
		close(client_fd); // Fermer la connexion du client en cas d'erreur
		return 0;
	}
	std::string hostname(host);
	// Créer un nouvel objet Client pour le nouveau client
	Client* newClient = new Client(client_fd, hostname);

	// Définir en tant qu'opérateur si c'est le premier client // ADMIN PLUTOT ??
	if (this->getClients().empty())
		newClient->setOp(true);

	// Ajouter le nouveau client au liste de clients
	//std::cout<<"ajout du nouveau client"<<std::endl;
	addClient(client_fd,newClient);
	// Ajouter le descripteur de fichier (socket) du nouveau client à l'instance epoll
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = client_fd;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client_fd, &event) != 0) {
		std::cerr << "Échec de l'ajout du descripteur de fichier du client à epoll." << std::endl;
		delete newClient; // Supprimer le client créé en cas d'échec
		close(client_fd); // Fermer la connexion du client en cas d'échec
		return 0;
	}
	return 1;
}

void	IRCServer::initCommands( void )
{
	//_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("CAP", &cap));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("NICK", &nick));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("KICK", &kick));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("INVITE", &invite));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("JOIN", &join));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("MODE", &mode));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("PRIVMSG", &privmsg));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("NAMES", &names));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("NOTICE", &notice));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("OPER", &oper));//on garde ?
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("PART", &part));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("PASS", &pass));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("PING", &ping));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("QUIT", &quit));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("TOPIC", &topic));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("USER", &user));
	//_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("WALLOPS", &wallops));//on garde ?
	//_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("WHOIS", &whois));
	_commands.insert(std::pair<std::string, int (*)(IRCServer&, Client&, std::vector<std::string>&)>("WHO", &who));
}
