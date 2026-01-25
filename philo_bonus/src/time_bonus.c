/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:54 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + ((tv.tv_usec + 500) / 1000));
}

void	ft_usleep(long ms)
{
	long	start;
	long	now;
	long	remaining;

	start = get_time_ms();
	while (1)
	{
		now = get_time_ms();
		if (now - start >= ms)
			break ;
		remaining = ms - (now - start);
		if (remaining > 1)
			usleep((remaining / 2) * 1000);
		else
			usleep(100);
	}
}
