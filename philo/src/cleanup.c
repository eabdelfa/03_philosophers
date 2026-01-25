/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:02:34 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_rules *rules)
{
	int	i;

	i = 0;
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
}

static void	destroy_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->meals_init)
	{
		pthread_mutex_destroy(&rules->philos[i].meal_mutex);
		i++;
	}
}

void	cleanup_rules(t_rules *rules)
{
	if (rules->forks)
	{
		destroy_forks(rules);
		free(rules->forks);
	}
	if (rules->philos)
	{
		destroy_philos(rules);
		free(rules->philos);
	}
	if (rules->stop_init)
		pthread_mutex_destroy(&rules->stop_mutex);
	if (rules->print_init)
		pthread_mutex_destroy(&rules->print_mutex);
}
