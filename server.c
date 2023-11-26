/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:21:54 by frapp             #+#    #+#             */
/*   Updated: 2023/11/26 17:22:49 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_sigusr1(int sig) {
	printf("Received SIGUSR1 signal\n");
}

void handle_sigusr2(int sig) {
	printf("Received SIGUSR2 signal\n");
}

int	main(void)
{
	// sig_t	test;

	// int test2;

	
	// test = signal(2, )

	int id = getpid();
	printf("%d\n", id);
	
	signal(SIGUSR1, handle_sigusr1);
	signal(SIGUSR2, handle_sigusr2);

	while (1)
		sleep(1);

	return (0);
}
     //30    SIGUSR1      terminate process    User defined signal 1
     //31    SIGUSR2      terminate process    User defined signal 2