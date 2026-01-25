/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	setup_philo_forks(t_rules *rules, int idx)
{
	rules->philos[idx].left = &rules->forks[idx];
	rules->philos[idx].right = &rules->forks[(idx + 1) % rules->num];
}

int	init_philos(t_rules *rules)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		rules->philos[philo_idx].id = philo_idx + 1;
		rules->philos[philo_idx].meals = 0;
		rules->philos[philo_idx].last_meal = -1;
		rules->philos[philo_idx].rules = rules;
		setup_philo_forks(rules, philo_idx);
		if (pthread_mutex_init(&rules->philos[philo_idx].meal_mutex, NULL) != 0)
		{
			print_error("failed to initialize philosopher meal mutex");
			while (--philo_idx >= 0)
				pthread_mutex_destroy(&rules->philos[philo_idx].meal_mutex);
			return (0);
		}
		rules->meals_init++;
		philo_idx++;
	}
	return (1);
}
