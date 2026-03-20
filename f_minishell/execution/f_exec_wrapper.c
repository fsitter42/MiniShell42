/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_exec_wrapper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:02:15 by fsitter           #+#    #+#             */
/*   Updated: 2026/03/20 14:08:50 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	f_pipeline_wrapper(t_data *data)
{
    int ret;
	t_cmd	*cmd;
	int		pipe_fd[2];
	int		prev_fd;

	cmd = data->cmds;
	prev_fd = -1;
	ret = f_exec_pipeline(data, cmd, pipe_fd, prev_fd);
    return (ret);
}
