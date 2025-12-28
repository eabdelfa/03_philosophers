/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:34 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 02:53:57 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** check_dead:
** Checks if the simulation's dead flag is set (thread-safe).
*/
bool	check_dead(t_data *data)
{
	bool	res;

	pthread_mutex_lock(&data->dead_lock);
	res = data->dead_flag;
	pthread_mutex_unlock(&data->dead_lock);
	return (res);
}

/*
** check_if_dead:
** Checks if any philosopher has died. Returns the philosopher's id if so,
** 0 otherwise.
*/
int	check_if_dead(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (get_time() - data->philos[i].last_meal_time >= data->time_to_die)
		{
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			return (i + 1);
		}
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (0);
}

/*
** check_if_all_ate:
** Checks if all philosophers have eaten the required number of times.
*/
int	check_if_all_ate(t_data *data)
{
	int	i;
	int	finished_eating;

	if (data->must_eat_count == -1)
		return (0);
	finished_eating = 0;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (data->philos[i].meals_eaten >= data->must_eat_count)
			finished_eating++;
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	if (finished_eating == data->nb_philos)
		return (1);
	return (0);
}

/*
** set_dead_flag:
** Sets the simulation's dead flag (thread-safe).
*/
void	set_dead_flag(t_data *data)
{
	pthread_mutex_lock(&data->dead_lock);
	data->dead_flag = true;
	pthread_mutex_unlock(&data->dead_lock);
}

/*
** monitor_routine:
** Monitor thread routine. Detects death or completion and prints status.
*/
void	*monitor_routine(void *pointer)
{
	t_data	*data;
	int		id;

	data = (t_data *)pointer;
	while (1)
	{
		id = check_if_dead(data);
		if (id != 0)
		{
			set_dead_flag(data);
			pthread_mutex_lock(&data->write_lock);
			printf("%lld %d died\n", get_time() - data->start_time, id);
			pthread_mutex_unlock(&data->write_lock);
			break ;
		}
		if (check_if_all_ate(data))
		{
			set_dead_flag(data);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
