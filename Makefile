NAME        = ircserv

CC            = c++
FLAGS        = -Wall -Wextra -Werror -g3 -std=c++98
RM            = rm -rf

INCLUDE        = includes

SRCSDIR        = srcs
OBJDIR         = .objFiles

FILES        =    main					\
                server/Server			\
				server/User				\
				server/OnUserEvent		\
				server/Channel			\
				commands/Command		\
				commands/pass			\
				commands/nick			\
				commands/user			\
				commands/quit			\
				commands/ping			\
				commands/part			\
				commands/kick			\
				commands/join			\
				commands/invite			\
				commands/topic			\
				commands/privmsg  \
				commands/mode			\
				utils					\


OBJ            = $(addprefix $(OBJDIR)/, $(FILES:=.o))

GREEN        =    \e[92m
YELLOW        =    \e[93m
RESET        =    \e[0m

ifeq ($(DEBUG), 1)
   OPTS = -g
endif

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -I $(INCLUDE) $(OPTS) -o $(NAME)
	@printf "$(_SUCCESS) $(GREEN)- Executable ready.\n$(RESET)"

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -I $(INCLUDE) $(OPTS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "$(YELLOW)	- Object files removed.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(YELLOW)	- Executable removed.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
