# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/21 18:25:14 by josjimen          #+#    #+#              #
#    Updated: 2026/08/03 20:11:41 by josjimen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -Iinclude

SRCS =	src/main.c \
		src/parser.c \
		src/time.c \
		src/init.c \
		src/destroy.c \
		src/routine.c \
		src/threads.c \
		src/state.c \
		src/dongle_init.c \
		src/request_priority.c \
		src/request_queue_init.c \
		src/request_heap.c \
		src/request_heap_utils.c \
		src/request_queue.c \
		src/dongle_access.c \
		src/request_wait.c \
		src/log.c \
		src/init_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re