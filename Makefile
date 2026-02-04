NAME        = ircserv
CPP         = c++
CPPFLAGS    = -Wall -Wextra -Werror -std=c++98
DIR_DUP     = mkdir -p $(@D)
RM          = rm -f
		
SRC_DIR     := src
OBJ_DIR     := obj

SRCS 		=	src/main.cpp \
				src/server.cpp \
				src/client.cpp \
				src/channel.cpp \
				src/cmd_pass.cpp \
				src/cmd_nick.cpp \
				src/cmd_user.cpp \
				src/cmd_privmsg.cpp \
				src/cmd_join.cpp \
				src/cmd_topic.cpp \
				src/cmd_part.cpp \
				src/cmd_mode.cpp \
				src/cmd_invite.cpp \
				src/cmd_kick.cpp \
				src/irc_utils.cpp \

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CYAN		= \033[36m
RESET		= \033[0m
UP			= \033[A #remonte le curseur d'une ligne dans le terminal
CUT			= \033[K #efface la ligne courante

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME) 
	@printf "$(CYAN)Compiling [$(NAME)]$(RESET)"
	@sleep 0.2 && printf "$(CYAN).$(RESET)" && sleep 0.2 && printf "$(CYAN).$(RESET)" && sleep 0.2 && printf "$(CYAN).$(RESET)\n"
	@printf "$(UP)$(CUT)"
	@printf "$(CYAN)Finished [$(NAME)]$(RESET)\n"
	@printf "$(UP)$(CUT)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(DIR_DUP)
	@$(CPP) $(CPPFLAGS) -c -o $@ $<

all: $(NAME)

clean:
	@$(RM) $(OBJS)
	@rm -rf $(OBJ_DIR)
	@printf "$(CYAN)Cleanup done.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re