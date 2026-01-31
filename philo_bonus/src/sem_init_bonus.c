/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_init_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/31 16:57:50 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*make_sem_name(char *base)
{
	char	*pid_str;
	char	*time_str;
	char	*tmp;
	char	*name;

	pid_str = ft_itoa((int)getpid());
	time_str = ft_itoa((int)(get_time_ms() % 100000));
	tmp = ft_strjoin(base, "_");
	name = ft_strjoin(tmp, pid_str);
	name = ft_strjoin(name, time_str);
	free(pid_str);
	free(time_str);
	free(tmp);
	return (name);
}

static void	cleanup_sem_names(t_rules *rules)
{
	free(rules->forks_name);
	free(rules->print_name);
	rules->forks_name = NULL;
	rules->print_name = NULL;
}

static int	create_forks_sem(t_rules *rules)
{
	rules->forks = sem_open(rules->forks_name, O_CREAT, 0644, rules->num);
	if (rules->forks == SEM_FAILED)
	{
		print_error("failed to create forks semaphore");
		cleanup_sem_names(rules);
		return (0);
	}
	return (1);
}

static int	create_print_sem(t_rules *rules)
{
	rules->print = sem_open(rules->print_name, O_CREAT, 0644, 1);
	if (rules->print == SEM_FAILED)
	{
		print_error("failed to create print semaphore");
		sem_close(rules->forks);
		sem_unlink(rules->forks_name);
		cleanup_sem_names(rules);
		return (0);
	}
	return (1);
}

int	open_sems(t_rules *rules)
{
	rules->forks_name = make_sem_name("/philo_forks");
	rules->print_name = make_sem_name("/philo_print");
	if (!rules->forks_name || !rules->print_name)
	{
		print_error("failed to generate semaphore names");
		cleanup_sem_names(rules);
		return (0);
	}
	sem_unlink(rules->forks_name);
	sem_unlink(rules->print_name);
	if (!create_forks_sem(rules))
		return (0);
	return (create_print_sem(rules));
}
