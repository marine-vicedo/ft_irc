#include "Ircserver.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Client::Client(int fd, std::string name) : 
	_fd(fd),
	_hostname(name),
	_username(""),
	_nickname(""),
	_realname(""),
	_partialInput(""),
	_isRegistered(false),
	_isAuth(false),
	_isOp(false),
	_isInvisible(false),
	_isWallops(false)
	{ }
Client::~Client() {
	//quitChannels();
}
void	Client::joinChannel(Channel *channel)
{
	channel->addUser(this);
	_channels.push_back(channel);
}
void Client::quitChannel(Channel *channel)
{
    if (!channel)
        return;
    std::vector<Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == channel)
        {
            break;
        }
    }
    if (it != _channels.end())
    {
        (*it)->removeUser(this);
        _channels.erase(it);
    }
}

void	Client::quitChannels() {
	std::vector<Channel*> tmp(_channels);
	for (std::vector<Channel*>::iterator it = tmp.begin(); it != tmp.end(); ++it)
		this->quitChannel(*it);
}

bool Client::isInChannel(const std::string& channelName) const {
        for (size_t i = 0; i < _channels.size(); ++i) {
            if (_channels[i]->getName() == channelName) {
                return true;
            }
        }
        return false;
    }
void Client::msg(const std::string &message) 
{
    if (_fd)
			send(_fd, message.c_str(), message.size(), 0);
}
/***************************getclient********************************/
        std::string Client::getHostname() { return _hostname; };
		std::string Client::getUsername() { return _username; };
		std::string Client::getNickname() { return _nickname; };
		std::string Client::getRealname() { return _realname; };
		std::string Client::getPartialInput() { return _partialInput; };
		bool 		Client::getRegistered() { return _isRegistered; };
		bool 		Client::getAuth() { return _isAuth; };
		bool 		Client::getOp() { return _isOp; };
		bool 		Client::getInvisible() { return _isInvisible; };
		bool 		Client::getWallops() { return _isWallops; };
/*******************************setclient**************************************/
        void 		Client::setFd(int &fd ) { _fd = fd; };
		void 		Client::setHostname(std::string const &hostname ) { _hostname = hostname; };
		void 		Client::setUsername(std::string const &username ) { _username = username; };
		void 		Client::setNickname(std::string const &nickname ) { _nickname = nickname; };
		void 		Client::setRealname(std::string const &realname ) { _realname = realname; };
		void 		Client::setPartialInput(std::string const &partialInput ) { _partialInput = partialInput; };
		void 		Client::setRegistered(bool isRegistered) { _isRegistered = isRegistered; };
		void 		Client::setAuth(bool isAuth) { _isAuth = isAuth; };
		void 		Client::setOp(bool isOp) { _isOp = isOp; };
		void 		Client::setInvisible(bool isInvisible) { _isInvisible = isInvisible; };
		void 		Client::setWallops(bool isWallops) { _isWallops = isWallops; };
/***************************************getchanell****************************************/
std::vector<Channel *> Client::getChannels() { return _channels; };