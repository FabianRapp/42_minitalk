/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:21:54 by frapp             #+#    #+#             */
/*   Updated: 2024/04/08 22:46:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// SUGYSR2 (ZERO)
// SIGUSR1 (ONE)

// server sends NORMAL_OP(0) means normal process
// server sends RESET(1) means ready to start (or restart);
////this is confirmed by client with ONE

// 8 * 0:
//		followed by ONE means print
//		foloowed by ZERO means new connection

#include "minitalk.h"

struct s_server	g_vars;

void	print_buffer(void);
void	first_signal(int sig, siginfo_t *siginfo, void *context);
void	reset_connection(int sig, siginfo_t *siginfo, void *context);

void	append_char(int sig)
{
	if (!g_vars.cmp)
	{
		g_vars.i++;
		g_vars.cmp = 0x80;
		g_vars.both_count = 0;
		g_vars.zero_count = 0;
	}
	if (g_vars.i == BUFFER_SIZE)
	{
		print_buffer();
	}
	if (sig == ONE)
		g_vars.buffer[g_vars.i] |= g_vars.cmp;
	else
	{
		g_vars.buffer[g_vars.i] &= ~g_vars.cmp;
		g_vars.zero_count++;
	}
	g_vars.cmp >>= 1;
	g_vars.both_count++;
}

void	print_buffer(void)
{
	int	end_offset;	

	end_offset = 3;
	while (end_offset)
	{
		if ((((g_vars.buffer[BUFFER_SIZE - end_offset] & 0xF0) == 0xF0)
				|| ((g_vars.buffer[BUFFER_SIZE - end_offset] & 0xE0) == 0xE0)
				|| ((g_vars.buffer[BUFFER_SIZE - end_offset] & 0xC0) == 0xC0)))
			break ;
		else
			end_offset--;
	}
	write(0, g_vars.buffer, BUFFER_SIZE - end_offset);
	g_vars.i = 0;
	while (end_offset)
	{
		g_vars.buffer[g_vars.i++] = g_vars.buffer[BUFFER_SIZE - end_offset];
		end_offset--;
	}
}

void	catch_binary(int sig)
{
	if (g_vars.zero_count >= 8)
	{
		if (sig == ONE)
		{
			write(0, g_vars.buffer, g_vars.i);
			write(0, "\n", 1);
		}
		g_vars.both_count = 0;
		g_vars.zero_count = 0;
		g_vars.i = 0;
		g_vars.cmp = 0x80;
		sigemptyset(&(g_vars.act.sa_mask));
		g_vars.act.sa_flags = SA_SIGINFO;
		g_vars.act.sa_sigaction = reset_connection;
		sigaction(ONE, &g_vars.act, NULL);
		sigaction(ZERO, &g_vars.act, NULL);
		return ;
	}
	append_char(sig);
	kill(g_vars.client_id, NORMAL_OP);
}

void	reset_connection(int sig, siginfo_t *siginfo, void *context)
{
	(void) context;
	g_vars.client_id = siginfo->si_pid;
	if (g_vars.zero_count >= 8)
	{
		sigemptyset(&(g_vars.act.sa_mask));
		g_vars.act.sa_flags = 0;
		g_vars.act.sa_handler = catch_binary;
		g_vars.zero_count = 0;
		g_vars.both_count = 0;
		g_vars.cmp = 0x80;
		g_vars.i = 0;
		sigaction(ONE, &g_vars.act, NULL);
		sigaction(ZERO, &g_vars.act, NULL);
		kill(g_vars.client_id, NORMAL_OP);
	}
	else if (sig == ONE)
	{
		g_vars.zero_count = 0;
		kill(g_vars.client_id, RESET);
	}
	else if (sig == ZERO)
	{
		g_vars.zero_count++;
		kill(g_vars.client_id, RESET);
	}
}

int	main(void)
{
	pid_t	id;

	id = getpid();
	ft_printf("%d\n", id);
	g_vars.zero_count = 0;
	g_vars.both_count = 0;
	g_vars.i = 0;
	g_vars.cmp = 0x80;
	sigemptyset(&(g_vars.act.sa_mask));
	g_vars.act.sa_flags = SA_SIGINFO;
	g_vars.act.sa_sigaction = reset_connection;
	g_vars.act.sa_flags = 0;
	sigaction(ONE, &g_vars.act, NULL);
	sigaction(ZERO, &g_vars.act, NULL);
	while (1)
	{
		pause();
	}
	return (0);
}

	// sigset_t	set;

	// sigemptyset(&set);
	// test = signal(2, )

	// signal(SIGUSR1, handle_sigusr1);
	// signal(SIGUSR2, handle_sigusr2);
