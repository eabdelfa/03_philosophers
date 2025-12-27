/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:18 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 18:45:37 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			print_error_and_exit("Error: Failed to create thread for \
				philosopher.\n");
		}
		i++;
	}
	return (0);
}

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

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		print_error_and_exit("[1] Usage: ./philo number_of_philosophers \
			time_to_die time_to_eat time_to_sleep\n\
			[number_of_times_each_philosopher_must_eat]\n");
	if (validate_args(argc, argv))
		print_error_and_exit("[1] Usage: ./philo number_of_philosophers \
			time_to_die time_to_eat time_to_sleep\n\
			[number_of_times_each_philosopher_must_eat]\n");
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
