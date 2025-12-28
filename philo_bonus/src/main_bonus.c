/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:07 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 02:37:12 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** cleanup:
** Frees all allocated resources and closes/unlinks all semaphores.
*/
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

/*
** wait_philos:
** Waits for all philosopher processes to finish, handles early termination
** on death.
*/
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
				data->stop_flag = 1;
				return ;
			}
			else if (WEXITSTATUS(status) == 0)
			{
				finished++;
			}
		}
	}
}

/*
** start_processes:
** Forks and starts all philosopher processes.
*/
int	start_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
		{
			ft_putstr_fd("Error: Failed to fork process for philosopher.\n", 2);
			exit(EXIT_FAILURE);
		}
		if (data->philos[i].pid == 0)
			philo_process(&data->philos[i]);
		i++;
	}
	return (0);
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
	if (init_semaphores(&data))
	{
		return (free(data.philos), 1);
	}
	data.stop_flag = 0;
	data.start_time = get_time();
	if (start_processes(&data))
		return (1);
	wait_philos(&data);
	cleanup(&data);
	return (0);
}
