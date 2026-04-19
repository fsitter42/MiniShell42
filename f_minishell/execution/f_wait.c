/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_wait.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 12:22:41 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/19 14:31:43 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// suwi
void	f_collect_status(t_data *data, int status)
{
	int	sig;

	sig = 0;
	if (WIFEXITED(status))
		data->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		else if (sig == SIGSEGV)
			ft_putendl_fd("Segmentation fault (core dumped)", 2);
		else if (sig == SIGINT)
			write(1, "\n", 1);
		data->last_exit_code = 128 + sig;
	}
}

void	f_wait_all(t_data *data)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	data->pids->i = 0;
	while (data->pids->i < data->pids->cc)
	{
		if (waitpid(data->pids->cpid[data->pids->i], &status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			else
			{
				return ;
			}
		}
		else
			f_collect_status(data, status);
		data->pids->i++;
	}
	f_setup_signals();
}
