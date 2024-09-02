#include "Ircserver.hpp"
#include <poll.h>
#include "Client.hpp"
#include "globals.hpp"
#include <sys/select.h>
/* nc localhost 9090
nick imen
user imraoui 0 * imraoui
join #ab*/
IRCServer::IRCServer(int port, std::string password) : serverSocket(-1), port(port), password(password),epollFd(-1) {
}

IRCServer::~IRCServer() {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
   	 	close(it->first); // it->first est le descripteur de fichier (fd)
    	delete it->second; // it->second est le pointeur vers l'objet Client
	}
	clients.clear(); 
for (std::vector<Channel*>::iterator it =chanells.begin(); it !=chanells.end(); ++it)
{
    delete *it; // Supprimer les objets Channel
}
chanells.clear();
close(serverSocket); 
}

IRCServer::IRCServer(const IRCServer &other) 
{
	*this = other;
}
IRCServer& IRCServer::operator=(const IRCServer &other)
{
   if(this!=&other)
   {
		this->serverSocket = other.serverSocket;
		this->port = other.port;
   }
   return(*this);
}

bool IRCServer::start()
{ 
	/************************creation de socket**********************************/ 
	//domain : la famille de protocoles à utiliser. Les valeurs possibles sont AF_INET, AF_INET6, AF_UNIX et AF_LOCAL.
	//type : le type de socket à créer. Les valeurs possibles sont SOCK_STREAM, SOCK_DGRAM et SOCK_RAW.
	//protocol : le protocole à utiliser. La valeur par défaut est 0, ce qui signifie que le système d'exploitation choisira le protocole le plus approprié.

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == -1) {
			std::cerr << "Erreur lors de la création du socket." << std::endl;
			return false;
		}
	/**********************CREATION adresse ip et du port*****************/
		struct sockaddr_in serverAddr;
	//initialiser tout les structure avce 0
		memset(&serverAddr, 0, sizeof(serverAddr));
	//indique adresse IPv4
		serverAddr.sin_family = AF_INET;
	//htons est utilisé pour convertir ce numéro de port en un format réseau standard.
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//htonl est utilisé pour convertir cette valeur en un format réseau standard.
		serverAddr.sin_port = htons(port);
	/**************************Liaison du socket*****************************/
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
			std::cerr << "Erreur lors de la liaison du socket." << std::endl;
		close(serverSocket);
		return false;
		}
	/******************* Écoute des connexions entrantes*******************/
	if (listen(serverSocket, SOMAXCONN) == -1) {
		std::cerr << "Erreur lors de l'écoute des connexions entrantes." << std::endl;
		close(serverSocket);
		return false;
	}

	std::cout << "Serveur IRC démarré. En attente de connexions sur le port " << port << std::endl;
	return true;
}
/****d'arrêter proprement le serveur IRC en fermant le socket du serveur****/
void IRCServer::stop() {
	if (serverSocket != -1) {
		close(serverSocket);
		std::cout << "Serveur IRC arrêté." << std::endl;
	}
}
void IRCServer::sendMessage(int clientSocket, std::string message) {
	send(clientSocket, message.c_str(), message.length(), 0);
}
/*
structure qui existe dans #include <sys/epoll.h>
struct epoll_event {
	uint32_t events;   // Événements surveillés
	epoll_data_t data; // Données associées à l'événement
};*/
int IRCServer::setupEpoll()
{
	setEpollFd(epoll_create1(0));
	if (epollFd == -1)
		throw std::runtime_error("Failed to create epoll file descriptor");

	struct epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = serverSocket;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event))
		throw std::runtime_error("Failed to add socket file descriptor to epoll");

	return epollFd;
}
   
void IRCServer::acceptConnections() 
{
	initCommands();
	 // Will contain client informatiochanellsn when new connection
	struct sockaddr_in client_address;
	// Calcule the len for accept()
	socklen_t client_len = sizeof(client_address);

	int epollFd = setupEpoll();

	// Create array of epoll events that will be filled by epoll_wait()
	struct epoll_event events[MAX_EVENTS];
	std::cout<<"epoll create : en attente d'evenements "<<std::endl;
	while (!g_shutdown) {
		// Wait for an event, block until event and if no event after 100ms return 0
		int num_fds = epoll_wait(epollFd, events, MAX_EVENTS, 100); // timeout of 100ms
		// Loop over all fd that had events
		for (int i = 0; i < num_fds; i++) {
			// Handle new connection // If event occured on server fd it means new connection
			if (events[i].data.fd == serverSocket)
				NewConnection(epollFd, client_address, client_len);
			// Handle data from a client // Else event occured on client fd
			else
			{
				NewMsg(events[i].data.fd);
			}
		}
	}
	close(epollFd);

}
