# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/22 23:04:18 by eabdelfa          #+#    #+#              #
#    Updated: 2026/01/26 18:16:27 by eabdelfa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -Iinclude

SRC_DIR = src
SRCS = main_bonus.c init_bonus.c routine_bonus.c monitor_bonus.c time_bonus.c utils_bonus.c cleanup_bonus.c start_bonus.c \
	   forks_bonus.c meal_bonus.c string_utils_bonus.c parsing_bonus.c sem_init_bonus.c number_utils_bonus.c print_bonus.c error_bonus.c sem_name_bonus.c
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
