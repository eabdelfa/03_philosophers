/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:02 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:13:40 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	make_sem_name(char *buffer, int id)
{
	int	i;
	int	len;

	i = 0;
	while ("/philo_meal_"[i])
	{
		buffer[i] = "/philo_meal_"[i];
		i++;
	}
	len = get_len(id);
	buffer[i + len] = '\0';
	while (len > 0)
	{
		buffer[i + len - 1] = (id % 10) + '0';
		id /= 10;
		len--;
	}
}

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
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (handle_error(5, 0), 1);
	return (0);
}

int	init_philo_sems(t_data *data)
{
	int		i;
	char	name[30];

	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		make_sem_name(name, i + 1);
		sem_unlink(name);
		data->philos[i].meal_sem = sem_open(name, O_CREAT, 0644, 1);
		if (data->philos[i].meal_sem == SEM_FAILED)
		{
			handle_error(6, i + 1);
			while (--i >= 0)
			{
				sem_close(data->philos[i].meal_sem);
				make_sem_name(name, i + 1);
				sem_unlink(name);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_semaphores(t_data *data)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	data->sem_forks = sem_open("/philo_forks", O_CREAT, 0644, data->nb_philos);
	data->sem_write = sem_open("/philo_write", O_CREAT, 0644, 1);
	if (data->sem_forks == SEM_FAILED || data->sem_write == SEM_FAILED)
	{
		handle_error(4, 0);
		if (data->sem_forks != SEM_FAILED)
			sem_close(data->sem_forks);
		if (data->sem_write != SEM_FAILED)
			sem_close(data->sem_write);
		sem_unlink("/philo_forks");
		sem_unlink("/philo_write");
		return (1);
	}
	if (init_philo_sems(data))
	{
		sem_close(data->sem_forks);
		sem_close(data->sem_write);
		sem_unlink("/philo_forks");
		sem_unlink("/philo_write");
		return (1);
	}
	return (0);
}
