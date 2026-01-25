/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:33 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static long	get_last_meal_time(t_philo *philo)
{
	long	last_meal_time;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (last_meal_time);
}

static void	check_philo_death(t_philo *philo, long current_time)
{
	if (current_time - philo->last_meal > philo->rules->t_die)
	{
		sem_wait(philo->rules->print);
		printf("%ld %d died\n", current_time - philo->rules->start, philo->id);
		exit(1);
	}
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	current_time;
	long	last_meal_time;

	philo = (t_philo *)arg;
	while (1)
	{
		last_meal_time = get_last_meal_time(philo);
		if (last_meal_time < 0)
		{
			usleep(1000);
			continue ;
		}
		current_time = get_time_ms();
		if (current_time - last_meal_time > philo->rules->t_die)
		{
			check_philo_death(philo, current_time);
		}
		usleep(1000);
	}
	return (NULL);
}
