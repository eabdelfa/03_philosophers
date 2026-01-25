/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, char *msg, int force)
{
	long	time;

	pthread_mutex_lock(&philo->rules->stop_mutex);
	if (!philo->rules->stop || force)
	{
		time = get_time_ms() - philo->rules->start;
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%ld %d %s\n", time, philo->id, msg);
		pthread_mutex_unlock(&philo->rules->print_mutex);
	}
	pthread_mutex_unlock(&philo->rules->stop_mutex);
}
