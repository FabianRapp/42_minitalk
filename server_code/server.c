/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:21:54 by frapp             #+#    #+#             */
/*   Updated: 2023/11/27 04:04:55 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"




// void handle_sigusr1(int sig)
// {
// 	static int count = 0;

// 	ft_printf("%d\n", sig);
// 	ft_printf("Received SIGUSR1 signal %d\n", ++count);
// }

// void handle_sigusr2(int sig)
// {
// 	static int	count = 0;

// 	ft_printf("%d\n", sig);
// 	ft_printf("Received SIGUSR2 signal %d\n", ++count);
// }



// void sigint_handler(int signum)
// {
// 	if (signum == 30)
// 		count++;
// 	else
// 	{
// 		buffer[i++] = count;
// 		if (count == 20)
// 		{
// 			write(1, buffer, i - 1);
// 			i = 0;
// 			write(1, "\n", 1);
// 		}
// 		else if (i == BUFFER_SIZE)
// 		{
// 			write(1, buffer, i);
// 			i = 0;
// 		}
// 		count = 20;
// 	}
// }



// volatile sig_atomic_t signal_received = 0;

// void signal_handler(int sig) {
//     signal_received = 1;
// }

// void function_to_run_if_no_signal()
// {
// 	count = 0;
// 	i = 0;
//     printf("No signal received for the given period.\n");
// }

void	zero_buffer(unsigned char *buffer)
{
	int	i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer[i++] = 0;
	}
}

unsigned char		buffer[BUFFER_SIZE];
int					i = -1;
unsigned char	cmp;
unsigned		sig_2_count;
unsigned char	both_sig_count = 8;
pid_t			client_id = 0;
bool			client_id_fin = false;


void	catch_binary(int signum)
{
	
	if (i == BUFFER_SIZE - 1)
	{
		write(1, buffer, BUFFER_SIZE - 1);
		zero_buffer(buffer);
		i = 0;
	}
	if (both_sig_count == 8)
	{
		sig_2_count = 0;
		both_sig_count = 0;
		cmp = 0x80;
		i++;
	}
	if (signum == 30)
	{
		buffer[i] = buffer[i] | cmp;
	}
	else
	{
		sig_2_count++;
	}
	both_sig_count++;
	cmp >>= 1;
	if (sig_2_count == 8)
	{
		write(1, buffer, i);
		write(1, "\n", 1);
		i = 0;
		while (i < BUFFER_SIZE)
		{
			buffer[i++] = 0;
		}
		client_id = 0;
		client_id_fin = false;
		both_sig_count = 8;
		i = -1;
	}
}

int main()
{
	
	struct sigaction act;
	printf("buffer size: %d\n", BUFFER_SIZE);
	act.sa_handler = catch_binary;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;
	pid_t id = getpid();
	printf("%d\n", id);

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	zero_buffer(buffer);
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