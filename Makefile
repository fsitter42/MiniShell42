# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:28:23 by slambert          #+#    #+#              #
#    Updated: 2026/03/20 14:06:56 by fsitter          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

# -- Dateien --

SRC_MAIN    = minishell.c

SRC_PARS    = b_minishell/commandizer/commandizer.c \
              b_minishell/cleanup/cleanup.c \
              b_minishell/tokenizer/tokenizer.c \
              b_minishell/expansion/expansion.c \
              b_minishell/word_splitting/word_splitting.c \
              b_minishell/s_execution/execution.c 

SRC_BUILTIN = f_minishell/builtin/cd/f_cd.c \
              f_minishell/builtin/echo/f_echo.c \
              f_minishell/builtin/env/f_env.c \
              f_minishell/builtin/export/f_export.c \
              f_minishell/builtin/export/f_export_key.c \
              f_minishell/builtin/export/f_export_no_key.c \
              f_minishell/builtin/pwd/f_pwd.c \
              f_minishell/builtin/unset/f_unset.c

# Environment Handling
SRC_ENVP    = f_minishell/envp/f_envp_init_destroy.c \
              f_minishell/envp/f_envp_to_lst.c \
              f_minishell/envp/f_lst_to_new_envp.c

# Execution & Pathfinder
SRC_EXE     = f_minishell/execution/f_exe.c \
              f_minishell/execution/f_exe2.c \
              f_minishell/execution/pathfinder/f_pathfinder.c \
              f_minishell/execution/pathfinder/f_pathhandler.c \
              f_minishell/execution/singlecmd/f_singlecmd.c \
              f_minishell/execution/b_heredoc.c \
              f_minishell/execution/f_exec_wrapper.c 

SRC         = $(SRC_MAIN) $(SRC_PARS) $(SRC_BUILTIN) $(SRC_ENVP) $(SRC_EXE)

OBJS        = $(SRC:.c=.o)

# -- Compiler & Flags --
CC          = cc
#CFLAGS      = -Wall -Wextra -Werror -g
CFLAGS      = -g
INCLUDE     = -I .

# -- Bibliotheken --
#LIBFT_PATH  = b_minishell/libft	
LIBFT_PATH  = f_includes/Libfs	#//frido libft
LIBFT       = $(LIBFT_PATH)/libft.a

# WICHTIG: -lreadline muss HINTER den Objekten stehen
LIBS        = -L$(LIBFT_PATH) -lft -lreadline

# -- Regeln --

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Kompilierung der einzelnen .o Dateien
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

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