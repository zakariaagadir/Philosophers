NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=thread

SRC = ft_atoi.c main.c philos.c routing.c check.c
OBJ = ${SRC:.c=.o}

all: ${NAME}


${NAME}: ${OBJ}
	${CC}  ${FLAGS} ${OBJ} -o ${NAME} 

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

clean:
	rm -f ${OBJ}

fclean: clean
	rm -f ${OBJ}
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
.SECONDARY:
