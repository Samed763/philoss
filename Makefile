NAME = philo
SRC = main.c utils.c control_and_insert.c 
OBJ = $(SRC:.c=.o)

all	: $(NAME) clean

$(NAME)	: $(OBJ)
	gcc -Wall -Wextra -Werror -g -pthread -o $(NAME) $(OBJ)

clean	:
	rm -f $(OBJ)

fclean	: clean
	rm -f $(NAME)

re	: fclean all

.PHONY	: all clean fclean re