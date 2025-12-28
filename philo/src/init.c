/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:07 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 01:48:35 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** init_data:
** Initializes the t_data structure with arguments and sets up mutexes.
*/
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
	{
		ft_putstr_fd("Error: Arguments must be positive.\n", 2);
		exit(EXIT_FAILURE);
	}
	data->dead_flag = false;
	data->forks = NULL;
	data->philos = NULL;
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0
		|| pthread_mutex_init(&data->write_lock, NULL) != 0)
	{
		ft_putstr_fd("Error: Mutex initialization failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	return (0);
}

/*
** init_forks:
** Allocates and initializes the fork mutexes for all philosophers.
*/
int	init_forks(t_data *data)
{
	int	i;
	int	j;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
	{
		ft_putstr_fd("Error: Memory allocation for forks failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			j = 0;
			while (j < i)
				pthread_mutex_destroy(&data->forks[j++]);
			free(data->forks);
			ft_putstr_fd("Error: Mutex initialization for fork failed.\n", 2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (0);
}

/*
** init_single_philo:
** Initializes a single philosopher's struct and its meal mutex.
*/
static int	init_single_philo(t_data *data, int i)
{
	t_philo	*philo;

	philo = &data->philos[i];
	philo->id = i + 1;
	philo->meals_eaten = 0;
	philo->last_meal_time = data->start_time;
	philo->data = data;
	philo->l_fork = &data->forks[i];
	philo->r_fork = &data->forks[(i + 1) % data->nb_philos];
	if (pthread_mutex_init(&philo->meal_lock, NULL) != 0)
		return (1);
	return (0);
}

/*
** init_philos:
** Allocates and initializes all philosopher structs.
*/
int	init_philos(t_data *data)
{
	int	i;
	int	j;

	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		ft_putstr_fd("Error: Memory allocation for philosophers failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (init_single_philo(data, i))
		{
			j = 0;
			while (j < i)
				pthread_mutex_destroy(&data->philos[j++].meal_lock);
			free(data->philos);
			ft_putstr_fd("Error: Mutex initialization for philosopher failed.\n",
				2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (0);
}
