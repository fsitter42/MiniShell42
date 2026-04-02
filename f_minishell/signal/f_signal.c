/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 12:05:46 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/02 12:41:28 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

static void	f_handle_sigint(int sig);

static void	f_handle_sigint(int sig)
{
	(void)sig;
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	f_setup_signals(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = f_handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
