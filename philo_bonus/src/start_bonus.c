/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:49 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/02/02 23:09:45 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_all(t_rules *rules)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		if (rules->pids[philo_idx] > 0)
			kill(rules->pids[philo_idx], SIGKILL);
		philo_idx++;
	}
}

static int	wait_children(t_rules *rules)
{
	int	exit_status;
	int	completed_count;
	int	child_pid;

	completed_count = 0;
	while (1)
	{
		child_pid = waitpid(-1, &exit_status, 0);
		if (child_pid <= 0)
			break ;
		if (WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == 1)
			return (1);
		if (WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == 0)
		{
			completed_count++;
			if (rules->has_must && completed_count == rules->num)
				return (2);
		}
	}
	return (0);
}

int	start_simulation(t_rules *rules)
{
	int		philo_idx;
	int		result;
	pid_t	child_pid;

	if (rules->has_must && rules->must_eat == 0)
		return (0);
	rules->start = get_time_ms();
	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		child_pid = fork();
		if (child_pid < 0)
		{
			print_error("failed to create child process");
			kill_all(rules);
			return (1);
		}
		if (child_pid == 0)
			child_process(rules, philo_idx + 1);
		rules->pids[philo_idx] = child_pid;
		philo_idx++;
	}
	result = wait_children(rules);
	if (result == 1 || result == 2)
		kill_all(rules);
	return (0);
}
