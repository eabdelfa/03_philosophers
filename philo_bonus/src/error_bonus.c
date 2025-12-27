/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:14:41 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 18:10:10 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, strlen(s));
}

void	print_error_and_exit(const char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

