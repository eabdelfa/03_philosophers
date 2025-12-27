/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:34 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:13:11 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_dead(t_data *data)
{
	bool	res;

	pthread_mutex_lock(&data->dead_lock);
	res = data->dead_flag;
	pthread_mutex_unlock(&data->dead_lock);
	return (res);
}

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

void	set_dead_flag(t_data *data)
{
	pthread_mutex_lock(&data->dead_lock);
	data->dead_flag = true;
	pthread_mutex_unlock(&data->dead_lock);
}

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
