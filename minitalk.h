/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:25:38 by frapp             #+#    #+#             */
/*   Updated: 2023/11/27 04:03:42 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "libft/libft.h"
#include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

#include "stdio.h"
#include <unistd.h>