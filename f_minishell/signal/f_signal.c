/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsitter <fsitter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 12:05:46 by fsitter           #+#    #+#             */
/*   Updated: 2026/04/07 12:04:52 by fsitter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	f_handle_sigint(int sig);
// static int f_sigint_event_hook(void);


// static int f_sigint_event_hook(void)
// {
//     if (g_signal_received == SIGINT)
//     {
//         rl_done = 1;
//     }
//     return (0);
// }

// static void f_handle_sigint(int sig)
// {
//     (void)sig;
//     g_signal_received = SIGINT;
// 	// write(1, "^C", 2);
// }

// void    f_setup_signals(void)
// {
//     struct sigaction    sa;

//     rl_catch_signals = 0;
//     rl_catch_sigwinch = 0;
//     rl_event_hook = f_sigint_event_hook;
//     ft_bzero(&sa, sizeof(sa));
//     sa.sa_handler = f_handle_sigint;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     sigaction(SIGINT, &sa, NULL);
//     signal(SIGQUIT, SIG_IGN);
// }



// static void	f_handle_sigint(int sig);

// static void	f_handle_sigint(int sig)
// {
// 	(void)sig;
// 	g_signal_received = SIGINT;
// 	// write(1, "^C\n", 3);
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	// rl_redisplay();
// 	rl_done = 1;
// }

// void	f_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	// rl_catch_signals = 0;
// 	ft_bzero(&sa, sizeof(sa));
// 	sa.sa_handler = f_handle_sigint;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }


// static void	f_handle_sigint(int sig);

// static void	f_handle_sigint(int sig)
// {
// 	(void)sig;
// 	g_signal_received = SIGINT;
// 	write(1, "^C\n", 3);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	f_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	rl_catch_signals = 0;
// 	ft_bzero(&sa, sizeof(sa));
// 	sa.sa_handler = f_handle_sigint;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }



// #include "minishell.h"

static int f_sigint_event_hook(void)
{
    if (g_signal_received == SIGINT)
    {
		write(1, "\n", 1);
    	rl_on_new_line();
    	rl_replace_line("", 0);
        rl_done = 1;
    }
    return (0);
}

static void f_handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = SIGINT;
    
}

void f_setup_signals(void)
{
    struct sigaction sa;

    rl_event_hook = f_sigint_event_hook;
    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = f_handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; 
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
