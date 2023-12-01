/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:53:33 by frapp             #+#    #+#             */
/*   Updated: 2023/12/01 05:46:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

struct s_client	g_vars = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

//void	end_msg(void);
void	end_msg(int sig);
void	reset_connection(void);

void	first_answer(int signum)
{
	if (signum == ZERO)
	{
		g_vars.zero_received = 1;
		g_vars.one_received = 0;
	}
	if (signum == ONE)
	{
		g_vars.zero_received = 0;
		g_vars.one_received = 1;
	}
}
// has race condition but never happens and brings performence

	// bool	one;
	// if (0x80 & *(str))
	// {
	// 	one = true; 
	// }
	// else
	// {
	// 	one = false;
	// }
	// if (one)
	// 	kill(server_id, ONE);
	// else
	// 	kill(server_id, ZERO);
void	send_data_bin(int sig)
{
	if (0x80 & *(g_vars.str))
		kill(g_vars.server_id, ONE);
	else
		kill(g_vars.server_id, ZERO);
	(*(g_vars.str)) <<= 1;
	g_vars.bit_i++;
	if (g_vars.bit_i >= 8)
	{
		g_vars.bit_i = 0;
		(g_vars.str)++;
		if (!(*g_vars.str))
		{
			g_vars.end_confirmations = 0;
			sigemptyset(&(g_vars.act.sa_mask));
			g_vars.act.sa_handler = end_msg;
			sigaction(ONE, &g_vars.act, NULL);
			sigaction(ZERO, &g_vars.act, NULL);
			return ;
		}
		//g_vars.i++;
	}
}

void	end_msg(int sig)
{
	if (sig == NORMAL_OP && g_vars.end_confirmations < 8)
	{
		kill(g_vars.server_id, ZERO);
		g_vars.end_confirmations++;
	}
	else if (g_vars.end_confirmations >= 8 && sig == NORMAL_OP)
	{
		kill(g_vars.server_id, ONE);
		g_vars.finished = true;
	}
	else
	{
		reset_connection();
	}
}

void	reset_connection(void)
{
	g_vars.i = 0;
	g_vars.zero_received = 0;
	g_vars.one_received = 0;
	sigemptyset(&(g_vars.act.sa_mask));
	g_vars.act.sa_handler = first_answer;
	sigaction(ONE, &g_vars.act, NULL);
	sigaction(ZERO, &g_vars.act, NULL);
	while (!g_vars.zero_received)
	{
		kill(g_vars.server_id, ZERO);
		usleep(1000);
	}
	g_vars.act.sa_handler = send_data_bin;
	sigaction(ONE, &g_vars.act, NULL);
	sigaction(ZERO, &g_vars.act, NULL);
}

int	main(int ac, char *av[])
{
	if (ac != 3)
	{
		printf("wrong ac!\n");
		return (0);
	}
	g_vars.server_id = atoi(av[1]);
	g_vars.str = (unsigned char *)av[2];
	reset_connection();
	send_data_bin(NORMAL_OP);
	while (!g_vars.finished)
	{
		pause();
	}
	return (0);
}
