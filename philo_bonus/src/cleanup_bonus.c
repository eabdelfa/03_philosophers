/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:56 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	cleanup_rules(t_rules *rules)
{
	if (rules->forks && rules->forks != SEM_FAILED)
		sem_close(rules->forks);
	if (rules->print && rules->print != SEM_FAILED)
		sem_close(rules->print);
	if (rules->forks_name)
	{
		sem_unlink(rules->forks_name);
		free(rules->forks_name);
	}
	if (rules->print_name)
	{
		sem_unlink(rules->print_name);
		free(rules->print_name);
	}
	if (rules->pids)
		free(rules->pids);
}
