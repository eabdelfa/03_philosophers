# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/20 00:05:13 by eabdelfa          #+#    #+#              #
#    Updated: 2025/12/27 13:16:26 by eabdelfa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS += -D LINUX
endif

CFLAGS += -I$(INCDIR)
SRCDIR = src
INCDIR = include
SRCS = $(SRCDIR)/main_bonus.c $(SRCDIR)/utils_bonus.c $(SRCDIR)/init_bonus.c $(SRCDIR)/routine_bonus.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
