/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:34 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/02/02 23:19:09 by eabdelfa         ###   ########.fr       */
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

static void	join_threads(t_rules *rules, int count)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < count)
	{
		pthread_join(rules->philos[philo_idx].thread, NULL);
		philo_idx++;
	}
}

int	start_simulation(t_rules *rules)
{
	int	threads_created;

	if (rules->has_must && rules->must_eat == 0)
	{
		set_stop(rules);
		return (0);
	}
	rules->start = get_time_ms();
	threads_created = create_threads(rules);
	if (threads_created != rules->num)
	{
		print_error("failed to create all philosopher threads");
		set_stop(rules);
		join_threads(rules, threads_created);
		return (1);
	}
	monitor_simulation(rules);
	join_threads(rules, rules->num);
	return (0);
}
