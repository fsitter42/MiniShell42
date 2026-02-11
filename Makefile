# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:28:23 by slambert          #+#    #+#              #
#    Updated: 2026/02/11 02:43:43 by slambert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC =	minishell.c
#BONUS_SRC = pipex_bonus.c pipex_utils1_bonus.c pipex_utils2_bonus.c \
			pipex_utils3_bonus.c

OBJS = ${SRC:.c=.o}
#BONUS_OBJS = $(BONUS_SRC:.c=.o)

CC = cc
RM = rm -f
#CFLAGS = -Wall -Wextra -Werror
CFLAGS = -lreadline
INCLUDE = -I .

LIBFT_PATH = libft
LIBFT = $(LIBFT_PATH)/libft.a
LIBS = -L $(LIBFT_PATH) -lft

.o:.c 
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o ${<:.c=.o}

$(NAME): 	$(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

#TODO stop bonus relinking
#bonus:		$(LIBFT) $(BONUS_OBJS)
#			$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

all:	${NAME}

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBJS)
#	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
#.PHONY: all clean fclean re bonus