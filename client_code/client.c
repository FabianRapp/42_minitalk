/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:53:33 by frapp             #+#    #+#             */
/*   Updated: 2023/11/27 04:18:55 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	send_data2(t_client *data)
{
	int		i;

	i = 0;
	while (*(data->str))
	{
		while (*(data->str))
		{
			(*(data->str))--;
			kill(data->server_id, SIGUSR1);
			usleep(150);
		}
		i++;
		if (i == BUFFER_SIZE)
		{
			usleep(400);
			i = 0;
		}
		kill(data->server_id, SIGUSR2);
		(data->str)++;
	}
	usleep(100);
	kill(data->server_id, SIGUSR2);
}

void	signal_end(t_client *data)
{
	int		i;

	i = 0;
	while (i < 10)
	{
		kill(data->server_id, SIGUSR2);
		usleep(250);
		i++;
	}
}

void	send_data_bin(t_client *data)
{
	int		i;
	int		bit_i;

	i = 0;
	while (*(data->str))
	{
		bit_i = 0;
		while (bit_i < 8)
		{
			if (0x80 & *(data->str))
			{
				//write(1, "1", 1);
				kill(data->server_id, SIGUSR1);
			}
			else
			{
				kill(data->server_id, SIGUSR2);
			}
			(*(data->str)) <<= 1;
			usleep(100);
			bit_i++;
		}
		(data->str)++;
		i++;
		if (i == BUFFER_SIZE)
		{
			usleep(200);
			i = 0;
		}
	}
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
	kill(data->server_id, SIGUSR2);
	usleep(100);
}



int main(int ac, char *av[])
{
	t_client	data;

	if (ac != 3)
	{
		printf("wrong ac!\n");
		return (0);	
	}
	data.server_id = atoi(av[1]);
	data.str =  (unsigned char *)av[2];
	send_data_bin(&data);

	return (0);
}
