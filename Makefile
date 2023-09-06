NAME = webserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS =	main.cpp\
		Server.cpp\
		WebServs.cpp

OBJS = $(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "webserv compiled!"

%.o:		%.cpp
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJS)

fclean:		clean
	@rm -f $(NAME)
	@echo "Everything cleaned!"

re:			fclean all

.PHONY: all clean fclean re