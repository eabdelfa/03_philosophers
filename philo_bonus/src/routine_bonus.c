/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:44 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping");
	ft_usleep(philo->rules->t_sleep);
}

static void	philo_think(t_philo *philo)
{
	long	think_time;

	print_state(philo, "is thinking");
	if (philo->rules->num % 2 == 1)
	{
		think_time = philo->rules->t_die - (philo->rules->t_eat
				+ philo->rules->t_sleep);
		if (think_time > 0)
			ft_usleep(think_time / 2);
	}
}

static int	setup_child_philo(t_rules *rules, int id, t_philo *philo)
{
	philo->id = id;
	philo->meals = 0;
	philo->last_meal = rules->start;
	philo->rules = rules;
	if (pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
	{
		print_error("failed to initialize philosopher meal mutex");
		exit(1);
	}
	if (pthread_create(&philo->monitor, NULL, &monitor_routine, philo) != 0)
	{
		print_error("failed to create philosopher monitor thread");
		pthread_mutex_destroy(&philo->meal_mutex);
		exit(1);
	}
	return (1);
}

static void	philo_loop(t_rules *rules, t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		pthread_mutex_lock(&philo->meal_mutex);
		if (rules->has_must && philo->meals >= rules->must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			exit(0);
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		philo_sleep(philo);
		philo_think(philo);
	}
}

void	child_process(t_rules *rules, int id)
{
	t_philo	philo;

	if (!setup_child_philo(rules, id, &philo))
		exit(1);
	philo_loop(rules, &philo);
}
