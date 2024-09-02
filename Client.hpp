#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Ircserver.hpp"
# include "Channel.hpp"
# include <vector>

class Channel;
class IRCServer;

class Client
{
	private:
		int						_fd;//Il s'agit probablement d'un descripteur de fichier pour représenter la connexion avec le client
		std::string				_hostname; // C'est le nom de l'ordinateur à partir duquel le client se connecte.
		std::string				_username; // Il représente le nom utilisé à des fins d'authentification et peut être unique pour chaque utilisateur.
		std::string				_nickname; // Il s'agit d'un nom utilisé pour identifier l'utilisateur dans le chat.
		std::string				_realname;//Ce champ contenir le vrai nom de l'utilisateur.
		std::string				_partialInput;//une variable utilisée pour stocker une entrée partielle du client avant de recevoir une entrée complète.
		bool					_isRegistered;//Un indicateur booléen indiquant si le client est enregistré.
		bool					_isAuth;//C'est un indicateur booléen pour représenter si le client est authentifié.
		bool					_isOp;// Indique si le client est un opérateur.
		bool					_isInvisible;// Indique si le client est invisible.
		bool					_isWallops;// Indique si le client reçoit les messages de type "wallops".
		std::string				_name;
		std::vector<Channel *>	_channels;

	public:
		Client(int fd, const std::string name);
		~Client();
		void msg(const std::string &message) ;
		int			getFd() { return _fd; };
		std::string getHostname() ;
		std::string getUsername(); 
		std::string getNickname() ;
		std::string getRealname() ;
		std::string getPartialInput() ;
		bool 		getRegistered() ;
		bool 		getAuth() ;
		bool 		getOp(); 
		bool 		getInvisible() ;
		bool 		getWallops() ;
		void 		setFd(int &fd ) ;
		void 		setHostname(std::string const &hostname ); 
		void 		setUsername(std::string const &username ) ;
		void 		setNickname(std::string const &nickname ) ;
		void 		setRealname(std::string const &realname ); 
		void 		setPartialInput(std::string const &partialInput ); 
		void 		setRegistered(bool isRegistered); 
		void 		setAuth(bool isAuth) ;
		void 		setOp(bool isOp); 
		void 		setInvisible(bool isInvisible); 
		void 		setWallops(bool isWallops); 
		std::vector<Channel *> getChannels() ;
		Channel*	getChannel(const std::string channelName);
		void		joinChannel(Channel *channel);
		void		quitChannel(Channel *channel);
		void		quitChannels();
		bool 		isInChannel(const std::string& channelName) const ;

};
#endif