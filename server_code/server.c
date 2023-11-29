/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:21:54 by frapp             #+#    #+#             */
/*   Updated: 2023/11/29 08:32:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minitalk.h"

#define ZERO SIGUSR2
#define ONE SIGUSR1

#define RESET ONE

#define NORMAL_OP ZERO


// SUGYSR2 (ZERO)
// SIGUSR1 (ONE)

// server sends NORMAL_OP(0) means normal process
// server sends RESET(1) means ready to start (or restart); this is confirmed by client with ONE

// 8 * 0:
//		followed by ONE means print
//		foloowed by ZERO means new connection

unsigned char		buffer[BUFFER_SIZE];
int					i;
unsigned char		cmp;
unsigned			zero_count;
unsigned char		both_count;
pid_t				client_id;
struct				sigaction act;

#include <fcntl.h>
int		fd;

void	first_signal(int sig, siginfo_t *siginfo, void *context);
void	reset_connection(int sig, siginfo_t *siginfo, void *context);

void	zero_buffer()
{
	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer[i++] = 0;
	}
	i = 0;
}

#define OUTPUTSTREAM 1

void	catch_binary(int signum)
{
	// happens extremly raly but still needs a check early since this is the exit condition
	if (zero_count >= 8)
	{
		if (signum == ONE)
		{
			write(OUTPUTSTREAM, buffer, i);
			write(OUTPUTSTREAM, "\n", 1);
		}
		both_count = 0;
		zero_count = 0;
		i = 0;
		cmp = 0x80;
		//zero_buffer();
		sigemptyset(&(act.sa_mask));
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = reset_connection;
		sigaction(ONE, &act, NULL);
		sigaction(ZERO, &act, NULL);
		return ;
	}
	// happens every 8th call
	if (!cmp)
	{
		i++;
		cmp = 0x80;
		both_count = 0;
		zero_count = 0;
	}
	// each 50 % of the time
	if (signum == ONE)
		buffer[i] |= cmp;
	else
	{
		buffer[i] &= ~cmp;
		zero_count++;
	}
	cmp >>= 1;
	both_count++;
	// happens every 20 0000 calls
	if (i == BUFFER_SIZE)
	{
		write(OUTPUTSTREAM, buffer, BUFFER_SIZE);
		//usleep(400);
		//zero_buffer();
		i = 0;
	}
	kill(client_id, NORMAL_OP);
}

void	reset_connection(int sig, siginfo_t *siginfo, void *context)
{
	client_id = siginfo->si_pid;
	if (zero_count >= 8)
	{
		//close (fd);
		fd = open("test1.txt", O_WRONLY | O_TRUNC);
		sigemptyset(&(act.sa_mask));
		act.sa_flags = 0;
		act.sa_handler = catch_binary;
		sigaction(ONE, &act, NULL);
		sigaction(ZERO, &act, NULL);
		zero_count = 0;
		both_count = 0;
		cmp = 0x80;
		i = 0;
		//zero_buffer();
		kill(client_id, NORMAL_OP);
	}
	else if (sig == ONE)
	{
		zero_count = 0;
		kill(client_id, RESET);
	}
	else if (sig == ZERO)
	{
		zero_count++;
		kill(client_id, RESET);
	}
}

int main()
{
	pid_t id = getpid();
	printf("%d\n", id);

	fd = open("test1.txt", O_WRONLY | O_TRUNC);
	zero_count = 0;
	both_count = 0;
	i = 0;
	cmp = 0x80;
	//act.sa_flags = SA_SIGINFO;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = reset_connection;

	act.sa_flags = 0;
	//act.sa_handler = ;

	sigaction(ONE, &act, NULL);
	sigaction(ZERO, &act, NULL);

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