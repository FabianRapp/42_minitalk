/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:21:54 by frapp             #+#    #+#             */
/*   Updated: 2023/11/28 07:08:54 by frapp            ###   ########.fr       */
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
int					i = -1;
unsigned char	cmp;
unsigned		zero_count;
unsigned char	both_count;
pid_t			client_id;
struct			sigaction act;

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

// void	catch_binary(int signum)
// {
// 	//usleep(400);
// 	ft_printf("catch bin\n");
// 	if (zero_count == 8)
// 	{
// 		if (signum == ONE)
// 		{
// 			ft_printf("should print\n");
// 			write(1, buffer, i);
// 			write(1, "\n", 1);
// 			i = 0;
// 			while (i < BUFFER_SIZE)
// 			{
// 				buffer[i++] = 0;
// 			}
// 		}
// 		else
// 			ft_printf("client reset_connection\n");
// 		//reset_connection();
// 		return ;
// 	}
// 	if (i == BUFFER_SIZE - 1)
// 	{
// 		write(1, buffer, BUFFER_SIZE - 1);
// 		zero_buffer();
// 		i = 0;
// 	}
// 	if (both_count == 8)
// 	{
// 		zero_count = 0;
// 		both_count = 0;
// 		cmp = 0x80;
// 		i++;
// 	}
// 	if (signum == ONE)
// 	{
// 		buffer[i] = buffer[i] | cmp;
// 	}
// 	else
// 	{
// 		zero_count++;
// 	}
// 	both_count++;
// 	cmp >>= 1;
// 	kill(client_id, NORMAL_OP);
// }


void	catch_binary(int signum)
{
	if (zero_count >= 8)
	{
		if (signum == ONE)
		{
			write(1, buffer, i);
			write(1, "\n", 1);
		}
		both_count = 0;
		zero_count = 0;
		i = 0;
		cmp = 0x80;
		zero_buffer();
		sigemptyset(&(act.sa_mask));
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = reset_connection;
		sigaction(ONE, &act, NULL);
		sigaction(ZERO, &act, NULL);
		return ;
	}
	if (signum == ONE)
	{
		buffer[i] = buffer[i] | cmp;
	}
	else
	{
		zero_count++;
	}
	cmp >>= 1;
	both_count++;
	if (!cmp && zero_count != 8)
	{
		both_count = 0;
		zero_count = 0;
		
	}
	if (!cmp)
	{
		i++;
		cmp = 0x80;
	}
	if (i == BUFFER_SIZE)
	{
		write(1, buffer, BUFFER_SIZE);
		write(1, "\n", 1);
		zero_buffer();
		i = 0;
	}
}

void	reset_connection(int sig, siginfo_t *siginfo, void *context)
{
	ft_printf("reset_connection, zero count: %d\n", zero_count);
	client_id = siginfo->si_pid;
	if (zero_count >= 8)
	{
		ft_printf("reset got ALL zero\n");
		sigemptyset(&(act.sa_mask));
		act.sa_flags = 0;
		act.sa_handler = catch_binary;
		sigaction(ONE, &act, NULL);
		sigaction(ZERO, &act, NULL);
		zero_count = 0;
		both_count = 0;
		cmp = 0x80;
		i = 0;
		zero_buffer();
		kill(client_id, NORMAL_OP);
	}
	// else if (sig == ONE && zero_count >= 8)
	// {
	// 	sigemptyset(&(act.sa_mask));
	// 	act.sa_flags = 0;
	// 	act.sa_handler = catch_binary;
	// 	sigaction(ONE, &act, NULL);
	// 	sigaction(ZERO, &act, NULL);
	// 	zero_count = 0;
	// 	both_count = 0;
	// 	kill(client_id, NORMAL_OP);
	// }
	else if (sig == ONE)
	{
		ft_printf("reset got one\n");
		zero_count = 0;
		kill(client_id, RESET);
	}
	else if (sig == ZERO)
	{
		ft_printf("reset got zero\n");
		zero_count++;
		kill(client_id, RESET);
	}
}

// void	reset_connection(void)
// {
// 	ft_printf("reset_connection\n");
// 	sigemptyset(&(act.sa_mask));
// 	zero_count = 0;
// 	both_count = 0;
// 	//act.sa_flags = SA_SIGINFO;
// 	act.sa_flags = SA_SIGINFO;
// 	act.sa_sigaction = first_signal;
// 	i = -1;
// 	//kill(client_id, RESET);
// }

//siginfo->si_pid
int main()
{
	pid_t id = getpid();
	printf("%d\n", id);

	//reset_connection();

	
	zero_count = 0;
	both_count = 0;
	i = 0;
	cmp = 0x80;
	//act.sa_flags = SA_SIGINFO;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = reset_connection;


	zero_buffer();
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