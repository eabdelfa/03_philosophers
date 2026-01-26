# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/22 23:03:02 by eabdelfa          #+#    #+#              #
#    Updated: 2026/01/26 18:17:11 by eabdelfa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -Iinclude

SRC_DIR = src
SRCS = main.c init.c routine.c monitor.c time.c utils.c cleanup.c start.c \
	   forks.c meal.c mutex.c philo_init.c print.c sync.c error.c parsing.c
OBJS = $(addprefix $(SRC_DIR)/,$(SRCS:.c=.o))
VPATH = $(SRC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -pthread $(OBJS) -o $(NAME)

$(SRC_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
