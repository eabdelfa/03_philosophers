/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:18 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 02:30:35 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** start_threads:
** Creates and starts all philosopher threads.
*/
int	start_threads(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL, &philo_routine,
				&data->philos[i]))
		{
			ft_putstr_fd("Error: Failed to create thread for philosopher.\n",
				2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (0);
}

/*
** join_threads:
** Joins all philosopher threads to ensure they finish before cleanup.
*/
void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

/*
** run_simulation:
** Runs the main simulation, including thread management and cleanup.
*/
int	run_simulation(t_data *data)
{
	pthread_t	monitor;

	if (start_threads(data))
	{
		cleanup(data);
		return (1);
	}
	if (pthread_create(&monitor, NULL, &monitor_routine, data))
	{
		set_dead_flag(data);
		join_threads(data);
		cleanup(data);
		return (1);
	}
	pthread_join(monitor, NULL);
	join_threads(data);
	cleanup(data);
	return (0);
}

/*
** is_numeric_str:
** Checks if a string contains only numeric characters.
*/
int	is_numeric_str(const char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

/*
** main:
** Entry point. Parses arguments, initializes data, and starts the simulation.
*/
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Usage:\n", 2);
		ft_putstr_fd("./philo_bonus number_of_philosophers time_to_die ", 2);
		ft_putstr_fd("time_to_eat time_to_sleep", 2);
		ft_putstr_fd(" [number_of_times_each_philosopher_must_eat]\n", 2);
		exit(EXIT_FAILURE);
	}
	if (validate_args(argc, argv))
		exit(EXIT_FAILURE);
	if (init_data(&data, argc, argv))
		return (1);
	if (init_forks(&data))
	{
		destroy_mutexes(&data);
		return (1);
	}
	if (init_philos(&data))
	{
		cleanup(&data);
		return (1);
	}
	return (run_simulation(&data));
}
