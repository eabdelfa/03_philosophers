/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:34 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/02/02 19:39:16 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_threads(t_rules *rules)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
		rules->philos[philo_idx].last_meal = rules->start;
		pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
		if (pthread_create(&rules->philos[philo_idx].thread, NULL,
				&philo_routine, &rules->philos[philo_idx]) != 0)
		{
			print_error("failed to create philosopher thread");
			return (philo_idx);
		}
		philo_idx++;
	}
	return (philo_idx);
}

int	start_simulation(t_rules *rules)
{
	int	philo_idx;
	int	threads_created;

	rules->start = get_time_ms();
	threads_created = create_threads(rules);
	if (threads_created != rules->num)
	{
		print_error("failed to create all philosopher threads");
		set_stop(rules);
		philo_idx = 0;
		while (philo_idx < threads_created)
		{
			pthread_join(rules->philos[philo_idx].thread, NULL);
			philo_idx++;
		}
		return (1);
	}
	monitor_simulation(rules);
	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		pthread_join(rules->philos[philo_idx].thread, NULL);
		philo_idx++;
	}
	return (0);
}
