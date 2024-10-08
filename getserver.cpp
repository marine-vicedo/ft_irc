#include "Ircserver.hpp"

class Client;
class channel;
class Ircserver;

std::string IRCServer::getServerName() const {
        return _name; 
}
std::map<int, Client*> IRCServer::getClients() const{
        return clients;
}
std::vector<Channel*> IRCServer::getChannel() const {
        return channels;
}

int IRCServer::getPort(){ return (port); }
std::string IRCServer::getPassword(){ return (password); }
void IRCServer::setPassword(std::string new_pwd){password = new_pwd;}
int IRCServer::getEpollFd(){ return (epollFd); }
void IRCServer::setEpollFd(int epoll){epollFd = epoll;}