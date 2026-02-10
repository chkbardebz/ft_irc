NAME        = ircserv
CPP         = c++
CPPFLAGS    = -Wall -Wextra -Werror -std=c++98
DIR_DUP     = mkdir -p $(@D)
RM          = rm -f
		
SRC_DIR     := src
OBJ_DIR     := obj

SRCS 		=	src/main.cpp \
				src/utils.cpp \
				src/network/server.cpp \
				src/network/client.cpp \
				src/network/channel.cpp \
				src/commands/cmd_pass.cpp \
				src/commands/cmd_nick.cpp \
				src/commands/cmd_user.cpp \
				src/commands/cmd_privmsg.cpp \
				src/commands/cmd_join.cpp \
				src/commands/cmd_topic.cpp \
				src/commands/cmd_part.cpp \
				src/commands/cmd_mode.cpp \
				src/commands/cmd_invite.cpp \
				src/commands/cmd_kick.cpp \
				src/commands/cmd_quit.cpp \

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME) 
	@printf "\n ██╗██████╗  ██████╗███████╗███████╗██████╗ ██╗   ██╗\n\
	 ██║██╔══██╗██╔════╝██╔════╝██╔════╝██╔══██╗██║   ██║\n\
	 ██║██████╔╝██║     ███████╗█████╗  ██████╔╝██║   ██║\n\
	 ██║██╔══██╗██║     ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n\
	 ██║██║  ██║╚██████╗███████║███████╗██║  ██║ ╚████╔╝ \n\
	 ╚═╝╚═╝  ╚═╝ ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  \n"
                                                    

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(DIR_DUP)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re