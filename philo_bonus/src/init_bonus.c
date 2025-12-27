/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:02 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 23:09:18 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** make_sem_name:
** Creates a unique semaphore name for each philosopher.
*/
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

/*
** init_data:
** Initializes the t_data structure with arguments and allocates philosophers.
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
		print_error_and_exit("Error: Arguments must be positive.\n");
	}
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		print_error_and_exit("Error: Memory allocation for philosophers \
			failed.\n");
	}
	return (0);
}

/*
** init_single_philo_sem:
** Initializes a single philosopher's semaphore for meal tracking.
*/
static int	init_single_philo_sem(t_data *data, int i, char *name)
{
	data->philos[i].id = i + 1;
	data->philos[i].meals_eaten = 0;
	data->philos[i].data = data;
	make_sem_name(name, i + 1);
	sem_unlink(name);
	data->philos[i].meal_sem = sem_open(name, O_CREAT, 0644, 1);
	if (data->philos[i].meal_sem == SEM_FAILED)
		return (1);
	return (0);
}

/*
** init_philo_sems:
** Initializes all philosopher semaphores and handles cleanup on failure.
*/
int	init_philo_sems(t_data *data)
{
	int		i;
	int		j;
	char	name[30];

	i = 0;
	while (i < data->nb_philos)
	{
		if (init_single_philo_sem(data, i, name))
		{
			j = 0;
			while (j < i)
			{
				make_sem_name(name, j + 1);
				sem_close(data->philos[j].meal_sem);
				sem_unlink(name);
			}
			free(data->philos);
			print_error_and_exit("Error: Semaphore initialization for \
				philosopher failed.\n");
		}
		i++;
	}
	return (0);
}

/*
** init_semaphores:
** Initializes the main semaphores for forks and writing,
** and all philosopher semaphores.
*/
int	init_semaphores(t_data *data)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	data->sem_forks = sem_open("/philo_forks", O_CREAT, 0644, data->nb_philos);
	data->sem_write = sem_open("/philo_write", O_CREAT, 0644, 1);
	if (data->sem_forks == SEM_FAILED || data->sem_write == SEM_FAILED)
	{
		print_error_and_exit("Error: Semaphore initialization failed.\n");
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
