NAME = philo_bonus
CC = cc
FLAGS = -Wall -Wextra -Werror

SRC = children_bonus.c ft_atoi_bonus.c ft_itoa_bonus.c\
 ft_strjoin_bonus.c ft_strlen_bonus.c main_bonus.c\
 routing_bonus.c routing2_bonus.c routing3_bonus.c
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
