define HEADER
                     webserv compiled!
██     ██ ███████ ██████  ███████ ███████ ██████  ██    ██
██     ██ ██      ██   ██ ██      ██      ██   ██ ██    ██
██  █  ██ █████   ██████  ███████ █████   ██████  ██    ██
██ ███ ██ ██      ██   ██      ██ ██      ██   ██  ██  ██
 ███ ███  ███████ ██████  ███████ ███████ ██   ██   ████


endef
export HEADER


NAME = webserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98
COLOR = \033[1;32m
RESET = `tput sgr0`

VPATH	= ./src/

SRCS =	main.cpp\
		Server.cpp\
		WebServs.cpp\
		Client.cpp \
		Request.cpp \
		Route.cpp \
		Response.cpp \
		Conf.cpp

OBJS = $(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(COLOR)$$HEADER"$(RESET)


%.o:		%.cpp
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJS)

fclean:		clean
	@rm -f $(NAME)
	@echo "                    $(COLOR)Everything cleaned!"$(RESET)

re:			fclean all

.PHONY: all clean fclean re
