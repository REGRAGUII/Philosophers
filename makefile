CC = cc -fsanitize=thread -g3
# CFLAGS = -Wall -Werror -Wextra
THREAD = -pthread

NAME = philo

SOURCE = main.c utils.c opt_sleep.c

all : $(NAME)

OBJ = $(SOURCE:.c=.o)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(THREAD) $(OBJ) -o $(NAME)
.c.o :
	$(CC) $(CFLAGS) $(THREAD) -c $< -o $@
fclean : clean
	rm -rf $(NAME)
clean :
	rm -rf $(OBJ)
re : fclean all