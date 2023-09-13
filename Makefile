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

VPATH	= ./src/

VPATH	= ./src/

SRCS =	main.cpp\
		Server.cpp\
		WebServs.cpp\
		Client.cpp \
		Request.cpp\
		Route.cpp

OBJS = $(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(COLOR)$$HEADER"


%.o:		%.cpp
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJS)

fclean:		clean
	@rm -f $(NAME)
	@echo "                    $(COLOR)Everything cleaned!"

re:			fclean all

.PHONY: all clean fclean re
