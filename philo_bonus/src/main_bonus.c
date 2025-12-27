/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:07 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 20:48:00 by eabdelfa         ###   ########.fr       */
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
** kill_all_philos:
** Sends SIGKILL to all philosopher processes.
*/
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

/*
** wait_philos:
** Waits for all philosopher processes to finish, handles early termination on death.
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
			print_error_and_exit("Error: Failed to fork process for \
				philosopher.\n");
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
		print_error_and_exit("Usage: ./philo_bonus number_of_philosophers \
			time_to_die time_to_eat time_to_sleep\n\
			[number_of_times_each_philosopher_must_eat]\n");
	if (validate_args(argc, argv))
		print_error_and_exit("Usage: ./philo_bonus number_of_philosophers \
			time_to_die time_to_eat time_to_sleep\n\
			[number_of_times_each_philosopher_must_eat]\n");
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
