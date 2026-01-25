/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:25 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping", 0);
	ft_usleep(philo->rules, philo->rules->t_sleep);
}

static void	philo_think(t_philo *philo)
{
	long	think_time;

	print_state(philo, "is thinking", 0);
	if (philo->rules->num % 2 == 1)
	{
		think_time = philo->rules->t_die - (philo->rules->t_eat
				+ philo->rules->t_sleep);
		if (think_time > 0)
			ft_usleep(philo->rules, think_time / 2);
	}
}

static void	solo_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	print_state(philo, "has taken a fork", 0);
	ft_usleep(philo->rules, philo->rules->t_die);
	pthread_mutex_unlock(philo->left);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->rules->num == 1)
	{
		solo_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!get_stop(philo->rules))
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
