# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:28:23 by slambert          #+#    #+#              #
#    Updated: 2026/04/19 15:07:57 by slambert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

# -- Dateien --

SRC_MAIN    = b_minishell/b_main/minishell.c \

SRC_PARS    = b_minishell/b_main/b_main_utils1.c \
              b_minishell/b_main/b_main_utils2.c \
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
              b_minishell/b_word_splitting/b_ws_token_stuff.c \
              b_minishell/b_word_splitting/b_ws_utils1.c \
              b_minishell/b_word_splitting/b_ws_utils2.c \
              b_minishell/b_execution/b_execution.c 

SRC_BUILTIN = f_minishell/builtin/cd/f_cd.c \
              f_minishell/builtin/echo/f_echo.c \
              f_minishell/builtin/env/f_env.c \
              f_minishell/builtin/export/f_export.c \
              f_minishell/builtin/export/f_export_key.c \
              f_minishell/builtin/export/f_export_no_key.c \
              f_minishell/builtin/pwd/f_pwd.c \
              f_minishell/builtin/unset/f_unset.c \
              f_minishell/builtin/exit/b_exit.c \
              f_minishell/builtin/exit/b_exit2.c

# Environment Handling
SRC_ENVP    = f_minishell/envp/f_envp_init_destroy.c \
              f_minishell/envp/f_envp_to_lst.c \
              f_minishell/envp/f_lst_to_new_envp.c

# Execution & Pathfinder
SRC_EXE     = f_minishell/execution/f_exe.c \
              f_minishell/execution/f_exe2.c \
              f_minishell/execution/f_exe3.c \
              f_minishell/execution/pathfinder/f_pathfinder.c \
              f_minishell/execution/pathfinder/f_pathhandler.c \
              f_minishell/execution/pathfinder/f_unsetpath.c \
              f_minishell/execution/singlecmd/f_singlecmd.c \
              f_minishell/execution/singlecmd/f_dot.c \
              f_minishell/execution/b_heredoc.c \
              f_minishell/execution/f_exec_wrapper.c \
              f_minishell/execution/f_exec_error.c \
              f_minishell/execution/f_wait.c
              

SRC_SIG     = f_minishell/signal/f_signal.c

SRC         = $(SRC_MAIN) $(SRC_PARS) $(SRC_BUILTIN) $(SRC_ENVP) $(SRC_EXE) $(SRC_SIG)

OBJS        = $(SRC:.c=.o)

# -- Compiler & Flags --
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDE     = -I .

# -- Bibliotheken --
LIBFT_PATH  = b_minishell/libft
LIBFT       = $(LIBFT_PATH)/libft.a

LIBFT_SRC   = $(LIBFT_PATH)/ft_tolower.c \
              $(LIBFT_PATH)/ft_toupper.c \
              $(LIBFT_PATH)/ft_isprint.c \
              $(LIBFT_PATH)/ft_isascii.c \
              $(LIBFT_PATH)/ft_isalnum.c \
              $(LIBFT_PATH)/ft_isdigit.c \
              $(LIBFT_PATH)/ft_isalpha.c \
              $(LIBFT_PATH)/ft_atoi.c \
              $(LIBFT_PATH)/ft_strlen.c \
              $(LIBFT_PATH)/ft_strlcat.c \
              $(LIBFT_PATH)/ft_strncmp.c \
              $(LIBFT_PATH)/ft_strdup.c \
              $(LIBFT_PATH)/ft_calloc.c \
              $(LIBFT_PATH)/ft_memcpy.c \
              $(LIBFT_PATH)/ft_strchr.c \
              $(LIBFT_PATH)/ft_strrchr.c \
              $(LIBFT_PATH)/ft_strnstr.c \
              $(LIBFT_PATH)/ft_bzero.c \
              $(LIBFT_PATH)/ft_memchr.c \
              $(LIBFT_PATH)/ft_memcmp.c \
              $(LIBFT_PATH)/ft_memset.c \
              $(LIBFT_PATH)/ft_memmove.c \
              $(LIBFT_PATH)/ft_strlcpy.c \
              $(LIBFT_PATH)/ft_putendl_fd_no_newline.c \
              $(LIBFT_PATH)/ft_itoa.c \
              $(LIBFT_PATH)/ft_strjoin.c \
              $(LIBFT_PATH)/ft_split.c \
              $(LIBFT_PATH)/ft_striteri.c \
              $(LIBFT_PATH)/ft_strmapi.c \
              $(LIBFT_PATH)/ft_substr.c \
              $(LIBFT_PATH)/ft_strtrim.c \
              $(LIBFT_PATH)/ft_putchar_fd.c \
              $(LIBFT_PATH)/ft_putstr_fd.c \
              $(LIBFT_PATH)/ft_putendl_fd.c \
              $(LIBFT_PATH)/ft_putnbr_fd.c \
              $(LIBFT_PATH)/ft_printf.c \
              $(LIBFT_PATH)/ft_putnbr_unsigned.c \
              $(LIBFT_PATH)/utils_hex.c \
              $(LIBFT_PATH)/get_next_line.c \
              $(LIBFT_PATH)/ft_lstnew.c \
              $(LIBFT_PATH)/ft_lstadd_front.c \
              $(LIBFT_PATH)/ft_lstsize.c \
              $(LIBFT_PATH)/ft_lstlast.c \
              $(LIBFT_PATH)/ft_lstadd_back.c \
              $(LIBFT_PATH)/ft_lstdelone.c \
              $(LIBFT_PATH)/ft_lstclear.c \
              $(LIBFT_PATH)/ft_lstiter.c \
              $(LIBFT_PATH)/ft_lstmap.c

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
$(LIBFT): $(LIBFT_SRC)
	$(MAKE) -C $(LIBFT_PATH)

all:	${NAME}

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
