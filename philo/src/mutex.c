/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_fork_mutexes(t_rules *rules)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		if (pthread_mutex_init(&rules->forks[philo_idx], NULL) != 0)
		{
			print_error("failed to initialize fork mutex");
			return (0);
		}
		rules->forks_init++;
		philo_idx++;
	}
	return (1);
}

int	init_mutexes(t_rules *rules)
{
	if (pthread_mutex_init(&rules->stop_mutex, NULL) != 0)
	{
		print_error("failed to initialize stop mutex");
		return (0);
	}
	rules->stop_init = 1;
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		print_error("failed to initialize print mutex");
		return (0);
	}
	rules->print_init = 1;
	return (init_fork_mutexes(rules));
}

void	cleanup_partial_init(t_rules *rules)
{
	int	idx;

	if (rules->forks)
	{
		idx = 0;
		while (idx < rules->forks_init)
		{
			pthread_mutex_destroy(&rules->forks[idx]);
			idx++;
		}
		free(rules->forks);
		rules->forks = NULL;
	}
	if (rules->stop_init)
	{
		pthread_mutex_destroy(&rules->stop_mutex);
		rules->stop_init = 0;
	}
	if (rules->print_init)
	{
		pthread_mutex_destroy(&rules->print_mutex);
		rules->print_init = 0;
	}
}
