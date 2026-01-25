/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:10 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 16:45:32 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	all_ate_enough(t_rules *rules)
{
	int	fed_philos;
	int	philo_idx;

	if (!rules->has_must)
		return (0);
	fed_philos = 0;
	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
		if (rules->philos[philo_idx].meals >= rules->must_eat)
			fed_philos++;
		pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
		philo_idx++;
	}
	if (fed_philos == rules->num)
		return (1);
	return (0);
}

static int	check_death(t_rules *rules)
{
	int		philo_idx;
	long	current_time;
	long	last_meal_time;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
		last_meal_time = rules->philos[philo_idx].last_meal;
		pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
		if (last_meal_time < 0)
		{
			philo_idx++;
			continue ;
		}
		current_time = get_time_ms();
		if (current_time - last_meal_time > rules->t_die)
		{
			set_stop(rules);
			print_state(&rules->philos[philo_idx], "died", 1);
			return (1);
		}
		philo_idx++;
	}
	return (0);
}

void	monitor_simulation(t_rules *rules)
{
	while (!get_stop(rules))
	{
		if (check_death(rules))
			return ;
		if (all_ate_enough(rules))
		{
			set_stop(rules);
			return ;
		}
		usleep(1000);
	}
}
