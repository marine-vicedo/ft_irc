#pragma once

// GENERAL
#define	SERVER_NAME "Ircserver"

#define REPLY_FORMAT(num_rply_numb, nickname) (std::string(":") + SERVER_NAME + " " + num_rply_numb + " " + nickname + " ")
#define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + SERVER_NAME + " " + command + " ")
#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
// 000
#define RPL_WELCOME(nickname) (std::string(":") + SERVER_NAME + " 001 " + nickname + " :Welcome to the  " + SERVER_NAME + " Server " + nickname + "\r\n")

// 200
#define RPL_UMODEIS(nickname, mode) (std::string(":") + SERVER_NAME + " 221 " + nickname + " " + mode + "\r\n")

// 300
#define RPL_WHOISUSER(nickname, username, hostname, realname) (std::string(":") + SERVER_NAME + " 311 " + nickname + " " + username + " " + hostname + " * :" + realname + "\r\n")
#define RPL_WHOISSERVER(nickname, servername, serverinfo) (std::string(":") + SERVER_NAME + " 312 " + nickname + " " + servername + " :" + serverinfo + "\r\n")
#define RPL_ENDOFWHOIS(nickname) (std::string(":") + SERVER_NAME + " 318 " + nickname + " :End of /WHOIS list" + "\r\n")
#define RPL_NOTOPIC(nickname, channel) (std::string(":") + SERVER_NAME + " 331 " + nickname + " #" + channel + " :No topic is set" + "\r\n")
#define RPL_TOPIC(nickname, channel, topic) (std::string(":") + SERVER_NAME + " 332 " + nickname + " #" + channel + " :" + topic + "\r\n")
#define ROL_TOPICWHOTIME(nickname, channel, concerned_client_nickname, time) (std::string(":") + SERVER_NAME + " 333 " + nickname + " " + channel + " " + concerned_client_nickname + " " + time + "\r\n")
#define RPL_INVITING(nickname, target, channel) (std::string(":") + SERVER_NAME + " 341 " + nickname + " " + target + " " + channel + "\r\n")
#define RPL_NAMREPLY(nickname, channel, list_client) (std::string(":") + SERVER_NAME + " 353 " + nickname + " = " + channel + " :" + list_client  + "\r\n")
#define RPL_ENDOFNAMES(nickname, channel) (std::string(":") + SERVER_NAME + " 366 " + nickname + " " + channel + " :End of /NAMES list" + "\r\n")
#define RPL_INFO(nickname) (std::string(":") + SERVER_NAME + " 371 " + nickname + " :" + SERVER_NAME + "\r\n" + \
							":" + SERVER_NAME + " 371 " + nickname + " :" + "written by hinda, imen, marine" + "\r\n" + \
							":" + SERVER_NAME + " 371 " + nickname + " :" + "version 1.0" + "\r\n")
#define RPL_ENDOFINFO(nickname) (std::string(":") + SERVER_NAME + " 374 " + nickname + " " + ":End of /INFO list. " + "\r\n")
#define RPL_YOUROPER(nickname) (std::string(":") + SERVER_NAME + " 381 " + nickname + " :You are now an IRC operator" + "\r\n")
// 300
#define RPL_WHOREPLY(nickname, channel, username, hostname, server, nick, mode, realname) (std::string(":") + SERVER_NAME + " 352 " + nickname + " " + channel + " " + username + " " + hostname + " " + server + " " + nick + " " + mode + " :0 " + realname + "\r\n")
#define RPL_ENDOFWHO(nickname, name) (std::string(":") + SERVER_NAME + " 315 " + nickname + " " + name + " :End of /WHO list.\r\n")

// 400

#define ERR_NOSUCHNICK(nickname, other_nickname) (std::string(":") + SERVER_NAME + " 401 " + nickname + " " + other_nickname + " :No such nick\r\n")
#define ERR_NOSUCHSERVER(nickname) (REPLY_FORMAT(" 402", nickname) + SERVER_NAME + " :" + ":No such server\r\n")
#define ERR_NOSUCHCHANNEL(nickname, channel) (std::string(":") + SERVER_NAME + " 403 " + nickname + " " + channel + " :No such channel" + "\r\n")
#define ERR_NOORIGIN(nickname) (std::string(":") + SERVER_NAME + " 409 " + nickname + " :Not enough parameters\r\n")
#define ERR_NONICKNAMEGIVEN(nickname) (std::string(":") + SERVER_NAME + " 431 " + nickname + " :No nickname given\r\n")
#define ERR_NICKNAMEINUSE(nickname) (std::string(":") + SERVER_NAME + " 433 " + nickname + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nickname, channel, concerned_client_nickname) (std::string(":") + SERVER_NAME + " 441 " + nickname + " " + concerned_client_nickname + " " + channel + " :They aren't on that channel" + "\r\n")
#define ERR_NOTONCHANNEL(nickname, channel) (std::string(":") + SERVER_NAME + " 442 " + nickname + " " + channel + " :You're not on that channel" + "\r\n")
#define ERR_USERONCHANNEL(nickname, channel, concerned_client_nickname) (std::string(":") + SERVER_NAME + " 443 " + nickname + " " + concerned_client_nickname + " #" + channel + " :is already on channel" + "\r\n")
#define ERR_NOTREGISTERED(nickname, command) (std::string(":") + SERVER_NAME + " 451 " + nickname + " " + command + " :You have not registered" + "\r\n")
#define ERR_INVITEONLYCHAN(nickname, channel) (std::string(":") + SERVER_NAME + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)" + "\r\n")
#define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVER_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(nickname) (std::string(":") + SERVER_NAME + " 462 " + nickname + " :Already registred\r\n")
#define ERR_PASSWDMISMATCH(nickname) (std::string(":") + SERVER_NAME + " 464 " + nickname + " :Password incorrect" + "\r\n")
#define ERR_CHANNELISFULL(nickname, channel) (REPLY_FORMAT("471", nickname) + channel + " :Cannot join channel (+l)" + "\r\n")
#define ERR_BADCHANNELKEY(nickname, channel) (REPLY_FORMAT("475", nickname) + channel + " :Cannot join channel (+k)" + "\r\n")
#define ERR_NOPRIVILEGES(nickname) (std::string(":") + SERVER_NAME + " 481 " + nickname + " :Permission Denied- You're not an IRC operator" + "\r\n")
#define ERR_CHANOPRIVSNEEDED(channel, nickname) (std::string(":") + SERVER_NAME + " 482 " + nickname + " " + channel + " :You're not channel operator" + "\r\n")

#define ERR_UMODEUNKNOWNFLAG(nickname) (std::string(":") + SERVER_NAME + " 501 " + nickname + " :Unknown MODE flag" + "\r\n")
#define ERR_USERSDONTMATCH(nickname) (std::string(":") + SERVER_NAME + " 502 " + nickname + " :Cant change mode for other users\r\n")

//macro CMDS

//NICK
#define NICK(nickname, username, new_nickname) (CLIENT_ID(nickname, username, "NICK") + ":" + new_nickname + "\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")

#define PONG(nickname) (std::string(":") + SERVER_NAME + " PONG " + SERVER_NAME + " :" + SERVER_NAME + "\r\n")
#define JOIN(nickname, username, channel) (CLIENT_ID(nickname, username, "JOIN") + ":#" + channel + "\r\n").c_str()
#define MODE_CHANNEL(nickname, username, channel, mode) (CLIENT_ID(nickname, username, "MODE") + "#" + channel + " " + mode + "\r\n")
#define INVITE(nickname, username, target, channel) (CLIENT_ID(nickname, username, "INVITE") + target + " :#" + channel + "\r\n")
#define TOPIC(nickname, username, channel, topic) (CLIENT_ID(nickname, username, "TOPIC") + "#" + channel + " :" + topic + "\r\n")
#define MODE_USER(nickname, username, target, mode) (CLIENT_ID(nickname, username, "MODE") + target + " " + mode + "\r\n")
#define PRIVMSG(nickname, username, dest, msg) (CLIENT_ID(nickname, username, "PRIVMSG") + dest + " :" + msg + "\r\n")
#define QUIT(nickname, username, message) (CLIENT_ID(nickname, username, "QUIT") + ":" + message + "\r\n")
#define NOTICE(nickname, username, dest, msg) (CLIENT_ID(nickname, username, "NOTICE") + dest + " :" + msg + "\r\n")
//PART
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")
# define KICK(nickname, username, channel, target, message) (CLIENT_ID(nickname, username, "KICK") + "#" + channel + " " + target + " :" + message + "\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")