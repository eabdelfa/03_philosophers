# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/20 00:04:25 by eabdelfa          #+#    #+#              #
#    Updated: 2025/12/27 13:20:58 by eabdelfa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

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
SRCS = $(SRCDIR)/main.c $(SRCDIR)/utils.c $(SRCDIR)/init.c $(SRCDIR)/routine.c $(SRCDIR)/monitor.c
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
