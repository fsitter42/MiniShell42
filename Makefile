# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:28:23 by slambert          #+#    #+#              #
#    Updated: 2026/04/03 16:24:17 by slambert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#TODO wenn nix zum ändern is "nothing to be made"
NAME        = minishell

# -- Dateien --

SRC_MAIN    = b_minishell/b_main/minishell.c \

SRC_PARS    = b_minishell/b_main/b_main_utils.c \
              b_minishell/b_commandizer/b_commandizer.c \
              b_minishell/b_commandizer/b_commandizer_utils1.c \
              b_minishell/b_commandizer/b_commandizer_utils2.c \
              b_minishell/b_commandizer/b_redirection.c \
              b_minishell/b_init/b_init.c \
              b_minishell/b_cleanup/b_cleanup1.c \
              b_minishell/b_cleanup/b_cleanup2.c \
              b_minishell/b_tokenizer/b_tokenizer.c \
              b_minishell/b_tokenizer/b_tok_utils.c \
              b_minishell/b_tokenizer/b_syntax_check.c \
              b_minishell/b_expansion/b_expansion.c \
              b_minishell/b_expansion/b_exp_env_var.c \
              b_minishell/b_expansion/b_exp_home_path.c \
              b_minishell/b_expansion/b_exp_quote.c \
              b_minishell/b_expansion/b_exp_utils.c \
              b_minishell/b_word_splitting/b_word_splitting.c \
              b_minishell/b_word_splitting/b_ws_utils.c \
              b_minishell/b_execution/b_execution.c 

SRC_BUILTIN = f_minishell/builtin/cd/f_cd.c \
              f_minishell/builtin/echo/f_echo.c \
              f_minishell/builtin/env/f_env.c \
              f_minishell/builtin/export/f_export.c \
              f_minishell/builtin/export/f_export_key.c \
              f_minishell/builtin/export/f_export_no_key.c \
              f_minishell/builtin/pwd/f_pwd.c \
              f_minishell/builtin/unset/f_unset.c \
              f_minishell/builtin/exit/b_exit.c

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

SRC_SIG     = f_minishell/signal/f_signal.c

SRC         = $(SRC_MAIN) $(SRC_PARS) $(SRC_BUILTIN) $(SRC_ENVP) $(SRC_EXE) $(SRC_SIG)

OBJS        = $(SRC:.c=.o)

# -- Compiler & Flags --
CC          = cc
#CFLAGS      = -Wall -Wextra -Werror -g
CFLAGS      = -Wall -g
INCLUDE     = -I .

# -- Bibliotheken --
LIBFT_PATH  = b_minishell/libft
#LIBFT_PATH  = f_includes/Libfs	#//frido libft
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

valgrind: re
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind_readline.supp --trace-children=yes --track-fds=yes ./minishell -d "ls"

.PHONY: all clean fclean re valgrind
