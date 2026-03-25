/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 13:22:51 by slambert          #+#    #+#             */
/*   Updated: 2026/03/25 15:23:09 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"


void b_exit (t_data *data, char **args, int* saved_fds)
{
    int exit_no;

    exit_no = 0;
    if (args[1])
        exit_no = ft_atoi(args[1]);
    sfbf_free_all(data);
    f_redir_restore(saved_fds, data);
    exit(exit_no);
}
