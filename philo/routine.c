/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:49 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 16:12:45 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** lock_forks:
** Implements asymmetric locking to prevent deadlocks.
** Even philosophers lock right then left.
** Odd philosophers lock left then right.
*/
void	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		print_msg("has taken a fork", philo);
		pthread_mutex_lock(philo->l_fork);
		print_msg("has taken a fork", philo);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_msg("has taken a fork", philo);
		pthread_mutex_lock(philo->r_fork);
		print_msg("has taken a fork", philo);
	}
}

void	eat(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_msg("has taken a fork", philo);
		ft_usleep(philo->data->time_to_die, philo->data);
		pthread_mutex_unlock(philo->l_fork);
		return ;
	}
	lock_forks(philo);
	pthread_mutex_lock(&philo->meal_lock);
	print_msg("is eating", philo);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1, philo->data);
	while (!check_dead(philo->data))
	{
		eat(philo);
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
			break ;
		print_msg("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		print_msg("is thinking", philo);
		if (philo->data->nb_philos % 2 != 0)
		{
			if (philo->data->time_to_eat >= philo->data->time_to_sleep)
				ft_usleep((philo->data->time_to_eat
						- philo->data->time_to_sleep) + 10, philo->data);
		}
	}
	return (NULL);
}
