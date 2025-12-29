/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:14:05 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 05:10:35 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** ft_strlen:
** Returns the length of a string.
*/
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

/*
** ft_putstr_fd:
** Writes a string to the given file descriptor.
*/
void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

/*
** destroy_mutexes:
** Destroys the main mutexes in the data structure.
*/
void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->write_lock);
}

/*
** cleanup:
** Frees all allocated resources and destroys all mutexes.
*/
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

/*
** usage:
** Displays the correct usage of the program.
*/
void	usage(void)
{
	ft_putstr_fd("Usage:\n", 2);
	ft_putstr_fd("./philo number_of_philosophers time_to_die ", 2);
	ft_putstr_fd("time_to_eat time_to_sleep ", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 2);
	ft_putstr_fd("  number_of_philosophers: 1 to 200\n", 2);
	ft_putstr_fd("  time_to_die, time_to_eat, time_to_sleep: > 60\n", 2);
	ft_putstr_fd("  If specified, ", 2);
	ft_putstr_fd("  [number_of_times_each_philosopher_must_eat] > 0\n", 2);
}
