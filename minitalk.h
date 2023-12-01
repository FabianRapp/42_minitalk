/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:25:38 by frapp             #+#    #+#             */
/*   Updated: 2023/12/01 07:10:30 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <unistd.h>
# include <signal.h>
# include "libft/libft.h"
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

# ifndef ZERO
#  define ZERO SIGUSR2
# endif

# ifndef ONE
#  define ONE SIGUSR1
# endif

# ifndef RESET
#  define RESET ONE
# endif

# ifndef NORMAL_OP
#  define NORMAL_OP ZERO
# endif

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

#endif
