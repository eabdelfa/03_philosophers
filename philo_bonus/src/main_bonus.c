/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:07 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/20 00:38:42 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	cleanup(t_data *data)
{
	int		i;
	char	name[30];

	sem_close(data->sem_forks);
	sem_close(data->sem_write);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	i = 0;
	while (i < data->nb_philos)
	{
		sem_close(data->philos[i].meal_sem);
		make_sem_name(name, i + 1);
		sem_unlink(name);
		i++;
	}
	free(data->philos);
}

void	kill_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}

void	wait_philos(t_data *data)
{
	int	status;
	int	finished;

	finished = 0;
	while (finished < data->nb_philos)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
			{
				kill_all_philos(data);
				return ;
			}
			else if (WEXITSTATUS(status) == 0)
			{
				finished++;
			}
		}
	}
}

int	start_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
		{
			kill_all_philos(data);
			cleanup(data);
			return (1);
		}
		if (data->philos[i].pid == 0)
			philo_process(&data->philos[i]);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (1);
	if (init_data(&data, argc, argv))
		return (1);
	if (init_semaphores(&data))
	{
		free(data.philos);
		return (1);
	}
	data.start_time = get_time();
	if (start_processes(&data))
		return (1);
	wait_philos(&data);
	cleanup(&data);
	return (0);
}
