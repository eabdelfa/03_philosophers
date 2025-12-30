/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:30 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/30 20:41:53 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	close_and_unlink_meal_sem(t_philo *philo)
{
	char	name[30];

	sem_close(philo->meal_sem);
	make_sem_name(name, philo->id);
	sem_unlink(name);
}

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
			close_and_unlink_meal_sem(philo);
			exit(EXIT_FAILURE);
		}
		sem_post(philo->meal_sem);
		usleep(1000);
	}
	return (NULL);
}

/*
** eat:
** Handles the eating routine for a philosopher,
** including fork/semaphore locking, eating, and unlocking.
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
** Main routine for each philosopher process. Handles the philosopher's
** life cycle.
*/
static void	philo_life_loop(t_philo *philo)
{
	while (1)
	{
		if (philo->data->stop_flag)
		{
			close_and_unlink_meal_sem(philo);
			exit(0);
		}
		eat(philo);
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
		{
			close_and_unlink_meal_sem(philo);
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

/*
** philo_process:
** Entry point for each philosopher process. Starts the monitor thread,
** applies a stagger for even philosophers, and runs the main life loop.
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
	philo_life_loop(philo);
}
