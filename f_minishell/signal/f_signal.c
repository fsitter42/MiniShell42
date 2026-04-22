/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 12:05:46 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/22 16:08:59 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	f_sigint_event_hook(void);
static void	f_handle_sigint(int sig);

static int	f_sigint_event_hook(void)
{
	if (g_signal_received == SIGINT)
	{
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
	}
	return (0);
}

static void	f_handle_sigint(int sig)
{
	g_signal_received = sig;
}

void	f_setup_child_signals(void)
{
	struct sigaction	sa;

	rl_signal_event_hook = f_sigint_event_hook;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = f_handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	f_setup_signals(void)
{
	struct sigaction	sa;

	rl_signal_event_hook = f_sigint_event_hook;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = f_handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	set_exit_code_to_130_and_free(t_data *data, char *line)
{
	g_signal_received = 0;
	data->last_exit_code = 130;
	if (line)
		free(line);
}

// static int	f_sigint_event_hook(void)
// {
// 	if (g_signal_received == SIGINT)
// 	{
// 		// rl_on_new_line();
// 		// rl_replace_line("", 0);
// 		// rl_done = 1;
// 		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
// 		rl_replace_line("", 0);
// 	}
// 	return (0);
// }

// static void	f_handle_sigint(int sig)
// {
// 	g_signal_received = sig;
// 	//write(1, "\n", 1);
// }