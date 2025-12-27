/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:30 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 20:48:00 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** monitor_routine:
** Monitor thread for a philosopher. Detects death and prints status.
*/
void	*monitor_routine(void *philo_ptr)
{
	t_philo		*philo;
	long long	time_since_last_meal;

	philo = (t_philo *)philo_ptr;
	while (1)
	{
		sem_wait(philo->meal_sem);
		time_since_last_meal = get_time() - philo->last_meal_time;
		if (time_since_last_meal >= philo->data->time_to_die)
		{
			sem_post(philo->meal_sem);
			sem_wait(philo->data->sem_write);
			printf("%lld %d died\n", get_time() - philo->data->start_time,
				philo->id);
			sem_close(philo->meal_sem);
			exit(1);
		}
		sem_post(philo->meal_sem);
		usleep(1000);
	}
	return (NULL);
}

/*
** eat:
** Handles the eating routine for a philosopher,
	including fork/semaphore locking, eating, and unlocking.
*/
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

/*
** philo_process:
** Main routine for each philosopher process. Handles the philosopher's life cycle.
*/
void	philo_process(t_philo *philo)
{
	pthread_t	monitor_thread;

	philo->last_meal_time = get_time();
	if (pthread_create(&monitor_thread, NULL, &monitor_routine, philo))
		exit(1);
	pthread_detach(monitor_thread);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 10);
	while (1)
	{
		eat(philo);
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
		{
			sem_close(philo->meal_sem);
			exit(0);
		}
		print_msg("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
		print_msg("is thinking", philo);
		if (philo->data->nb_philos % 2 != 0)
		{
			if (philo->data->time_to_eat >= philo->data->time_to_sleep)
				ft_usleep((philo->data->time_to_eat
						- philo->data->time_to_sleep) * 1.1);
		}
	}
}
