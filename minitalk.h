/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:25:38 by frapp             #+#    #+#             */
/*   Updated: 2023/12/01 06:53:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "libft/libft.h"
#include <stdbool.h>


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

#define ZERO SIGUSR2
#define ONE SIGUSR1
#define RESET ONE
#define NORMAL_OP ZERO

struct	s_client
{
	pid_t					server_id;
	struct sigaction		act;
	unsigned char			*str;
	int						sleep_time1;
	int						sleep_time2;
	int						bit_i;
	bool					finished;
	volatile sig_atomic_t	zero_received;
	volatile sig_atomic_t	one_received;
	int						i;
	int						end_confirmations;
} ;

struct	s_server
{
	unsigned char		buffer[BUFFER_SIZE];
	int					i;
	unsigned char		cmp;
	unsigned char		zero_count;
	unsigned char		both_count;
	pid_t				client_id;
	struct sigaction	act;
} ;

#include "stdio.h"
#include <unistd.h>