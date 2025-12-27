/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:07 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:13:06 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	if (data->nb_philos <= 0 || data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0)
		return (handle_error(2, 0), 1);
	data->dead_flag = false;
	data->forks = NULL;
	data->philos = NULL;
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0
		|| pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (handle_error(4, 0), 1);
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (handle_error(5, 0), 1);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			handle_error(6, i);
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			data->forks = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (handle_error(7, 0), 1);
	i = 0;
	while (i < data->nb_philos)
	{
		philo = &data->philos[i];
		philo->id = i + 1;
		philo->meals_eaten = 0;
		philo->last_meal_time = data->start_time;
		philo->data = data;
		philo->l_fork = &data->forks[i];
		philo->r_fork = &data->forks[(i + 1) % data->nb_philos];
		if (pthread_mutex_init(&philo->meal_lock, NULL) != 0)
		{
			handle_error(8, i + 1);
			while (--i >= 0)
				pthread_mutex_destroy(&data->philos[i].meal_lock);
			free(data->philos);
			data->philos = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philos)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
	}
	if (data->philos)
	{
		i = 0;
		while (i < data->nb_philos)
			pthread_mutex_destroy(&data->philos[i++].meal_lock);
		free(data->philos);
	}
	destroy_mutexes(data);
}
