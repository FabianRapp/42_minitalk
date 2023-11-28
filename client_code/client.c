/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:53:33 by frapp             #+#    #+#             */
/*   Updated: 2023/11/28 09:49:01 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minitalk.h"


#define ZERO SIGUSR2
#define ONE SIGUSR1

#define RESET ONE

#define NORMAL_OP ZERO

pid_t			server_id = 0;
struct			sigaction act;
unsigned char	*str;
int				sleep_time1;
int				sleep_time2;
int				bit_i;
bool			finished = false;
volatile sig_atomic_t zero_received = 0;
volatile sig_atomic_t one_received = 0;
int				i;
int				bit_i;
int				end_confirmations;

//void	end_msg(void);
void	end_msg(int sig);
void	reset_connection(void);

void first_answer(int signum)
{
	if (signum == ZERO)
	{
		zero_received = 1;
		one_received = 0;
	}
	if (signum == ONE)
	{
		zero_received = 0;
		one_received = 1;
	}
}
// has race condition but never happens and brings performence
void	send_data_bin(int sig)
{
	if (0x80 & *(str))
	{
		kill(server_id, ONE);
	}
	else
	{
		kill(server_id, ZERO);
	}
	(*(str)) <<= 1;
	bit_i++;
	if (bit_i >= 8)
	{
		bit_i = 0;
		(str)++;
		if (!(*str))
		{
			end_confirmations = 0;
			sigemptyset(&(act.sa_mask));
			act.sa_handler = end_msg;
			sigaction(ONE, &act, NULL);
			sigaction(ZERO, &act, NULL);
			return ;
		}
		i++;
	}
}


void	end_msg(int sig)
{
	//printf("end msg %d\n", end_confirmations + 1);
	if (sig == NORMAL_OP && end_confirmations < 8)
	{
		kill(server_id, ZERO);
		end_confirmations++;
	}
	else if (end_confirmations >= 8 && sig == NORMAL_OP)
	{
		kill(server_id, ONE);
		finished = true;
	}
	else
	{
		reset_connection();
	}
}

void	reset_connection(void)
{
	i = 0;
	zero_received = 0;
	one_received = 0;
	sigemptyset(&(act.sa_mask));
	act.sa_handler = first_answer;
	sigaction(ONE, &act, NULL);
	sigaction(ZERO, &act, NULL);
	//ft_printf("reset_connection started\n");

	while (!zero_received)
	{
		kill(server_id, ZERO);
		usleep(1000);
	}
	act.sa_handler = send_data_bin;
	sigaction(ONE, &act, NULL);
	sigaction(ZERO, &act, NULL);
	//ft_printf("reset_connection finished\n");
}


int main(int ac, char *av[])
{
	if (ac != 3)
	{
		printf("wrong ac!\n");
		return (0);	
	}


	server_id = atoi(av[1]);
	str =  (unsigned char *)av[2];

	reset_connection();
	send_data_bin(NORMAL_OP);

	while (!finished)
	{
		pause();
	}
	return (0);
}
