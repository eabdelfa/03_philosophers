/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:30 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/20 00:38:42 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	while (1)
	{
		sem_wait(philo->meal_sem);
		if (get_time() - philo->last_meal_time >= philo->data->time_to_die)
		{
			sem_post(philo->meal_sem);
			sem_wait(philo->data->sem_write);
			printf("%lld %d died\n", get_time() - philo->data->start_time,
				philo->id);
			exit(1);
		}
		sem_post(philo->meal_sem);
		usleep(1000);
	}
	return (NULL);
}

void	eat(t_philo *philo)
{
	sem_wait(philo->data->sem_forks);
	print_msg("has taken a fork", philo);
	sem_wait(philo->data->sem_forks);
	print_msg("has taken a fork", philo);
	sem_wait(philo->meal_sem);
	print_msg("is eating", philo);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	sem_post(philo->meal_sem);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->sem_forks);
	sem_post(philo->data->sem_forks);
}

void	philo_process(t_philo *philo)
{
	pthread_t	monitor;

	philo->last_meal_time = get_time();
	if (pthread_create(&monitor, NULL, &monitor_routine, philo))
		exit(1);
	pthread_detach(monitor);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 10);
	while (1)
	{
		eat(philo);
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
			exit(0);
		print_msg("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
		print_msg("is thinking", philo);
	}
}
