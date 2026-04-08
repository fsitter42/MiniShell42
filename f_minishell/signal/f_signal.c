/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 12:05:46 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/08 15:13:20 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	f_sigint_event_hook(void);
static void	f_handle_sigint(int sig);

static int	f_sigint_event_hook(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

static void	f_handle_sigint(int sig)
{
	g_signal_received = sig;
	write(1, "\n", 1);
}

void	f_setup_signals(void)
{
	struct sigaction	sa;

	rl_event_hook = f_sigint_event_hook;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = f_handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
