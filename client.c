/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:53:33 by frapp             #+#    #+#             */
/*   Updated: 2024/04/08 22:37:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

struct s_client	g_vars = {0, {}, 0, 0, 0, 0, 0, 0, 0, 0,};

// if the server allready has a buffered message it will be corrupted and
// discarded from a new client connection

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

// to avoid theroretical data races:
// move "kill" to the end of the function
void	send_data_bin(int sig)
{
	sig++;
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
			// if (0x80 & *(g_vars.str)) // more save
			// 	kill(g_vars.server_id, ONE); // but slightly slower
			// else
			// 	kill(g_vars.server_id, ZERO);
			return ;
		}
	}
	// if (0x80 & *(g_vars.str))
	// 	kill(g_vars.server_id, ONE);
	// else
	// 	kill(g_vars.server_id, ZERO);
}

// tells the server to print or discard the current buffer
void	end_msg(int sig)
{
	if (sig == NORMAL_OP && g_vars.end_confirmations < 8)
	{
		g_vars.end_confirmations++;
		kill(g_vars.server_id, ZERO);
	}
	else if (g_vars.end_confirmations >= 8 && sig == NORMAL_OP)
	{
		g_vars.finished = true;
		kill(g_vars.server_id, ONE);
	}
	else
	{
		reset_connection();
	}
}

// makes sure the server is not in a corrupted state
// keeps sending "0" to the server which should indicate
//	the server eventually a (new) client
// after the server acknowledged the client transfer starts
// the server will send exactly one zero before the data transfer
void	reset_connection(void)
{
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
		ft_printf("wrong ac!\n");
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
