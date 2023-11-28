/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:53:33 by frapp             #+#    #+#             */
/*   Updated: 2023/11/28 07:06:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

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

void	end_msg(void);
void	reset_connection(void);

void first_answer(int signum)
{
	if (signum == ZERO)
	{
		zero_received = 1;
	}
	if (signum == ONE)
	{
		one_received = 1;
	}
}

void	signal_end(t_client *data)
{
	int		i;

	i = 0;
	while (i < 10)
	{
		kill(server_id, SIGUSR2);
		usleep(250);
		i++;
	}

}

void	send_data_bin(int sig)
{
	int		i;
	int		bit_i;

	i = 0;
	while (*(str))
	{
		bit_i = 0;
		while (bit_i < 8)
		{
			if (0x80 & *(str))
			{
				//write(1, "1", 1);
				kill(server_id, ONE);
			}
			else
			{
				kill(server_id, ZERO);
			}
			(*(str)) <<= 1;
			usleep(200);
			bit_i++;
		}
		(str)++;
		i++;
		if (i == BUFFER_SIZE + 1)
		{
			usleep(2000);
			i = 0;
		}
	}
	end_msg();
	// kill(server_id, ONE);
	// usleep(2000);
}

// void	send_data_bin(int signum)
// {
// 	int		i;

// 	//usleep(400);

// 	//if (signum == NORMAL_OP)
// 	while(*(str))
// 	{
// 		i = 0;
// 		if (*(str))
// 		{
// 			if (bit_i >= 8)
// 			{
// 				bit_i = 0;
// 				(str)++;
// 				i++;
// 			}
// 			if (0x80 & *(str))
// 			{
// 				(*(str)) <<= 1;
// 				bit_i++;
// 				//write(1, "1", 1);
// 				kill(server_id, ONE);
// 			}
// 			else
// 			{
// 				(*(str)) <<= 1;
// 				bit_i++;
// 				kill(server_id, ZERO);
// 			}
// 			//usleep(400);
// 			// if (i == BUFFER_SIZE)
// 			// {
// 			// 	//usleep(800);
// 			// 	i = 0;
// 			// }
// 		}
// 		else
// 		{
// 			end_msg();
// 		}
// 	}
// 	else if (!finished)
// 	{
// 		reset_connection();
// 	}
// }

void	end_msg(void)
{
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);
	kill(server_id, ZERO);
	usleep(2000);

	// 9th to say print:
	kill(server_id, ONE);
	usleep(2000);

	finished = true;
}


void	reset_connection(void)
{
	int		reset_sleep_time = 400;

	zero_received = 0;
	one_received = 0;
	sigemptyset(&(act.sa_mask));
	act.sa_handler = first_answer;
	sigaction(ONE, &act, NULL);
	sigaction(ZERO, &act, NULL);
	ft_printf("reset_connection started\n");

	while (!zero_received)
	{
		kill(server_id, ZERO);
		usleep(2000);
	}
	
	usleep(2000);
	// act.sa_handler = send_data_bin;
	sigaction(ONE, &act, NULL);
	sigaction(ZERO, &act, NULL);
	ft_printf("reset_connection finished\n");
}

void	first_signal(int signum)
{
	if (signum == NORMAL_OP)
	{
		act.sa_handler = send_data_bin;
		send_data_bin(NORMAL_OP);
		//kill(server_id, SIGUSR2);
	}
	else
	{
		reset_connection();
	}
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

	// while (!finished)
	// {
	// 	pause();
	// }
	return (0);
}
