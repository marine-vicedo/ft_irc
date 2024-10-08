SRCS = main.cpp \
       Channel.cpp \
       Client.cpp \
       getserver.cpp \
       IRCCommandParser.cpp \
       Ircserver.cpp \
       ./commands/cap.cpp \
       ./commands/bot.cpp \
       ./commands/die.cpp \
       ./commands/invite.cpp \
       ./commands/join.cpp \
       ./commands/kick.cpp \
       ./commands/mode.cpp \
       ./commands/names.cpp \
       ./commands/nick.cpp \
       ./commands/notice.cpp \
       ./commands/oper.cpp \
       ./commands/part.cpp \
       ./commands/pass.cpp \
       ./commands/ping.cpp \
       ./commands/privmsg.cpp \
       ./commands/quit.cpp \
       ./commands/topic.cpp \
       ./commands/user.cpp \
       ./commands/who.cpp

HEADERS = Channel.hpp \
          Client.hpp \
          commands.hpp \
          define.hpp \
          globals.hpp \
          Ircserver.hpp


OBJS	= ${SRCS:.cpp=.o}

NAME	= ircserv

CC		= c++

CFLAGS	= -Wall -Werror -Wextra -std=c++98

RM		= rm -rf

.cpp.o:
		${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}

all:	$(NAME)

$(NAME): ${OBJS} ${HEADERS}	
		${CC} ${CFLAGS} ${OBJS}  -o ${NAME}

clean:	
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re:		fclean all

.PHONY: all re clean fclean