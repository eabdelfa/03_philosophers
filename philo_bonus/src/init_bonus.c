/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:04 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_rules(t_rules *rules, int ac, char **av)
{
	if (!parse_rules(rules, ac, av))
		return (0);
	rules->pids = malloc(sizeof(pid_t) * rules->num);
	if (!rules->pids)
	{
		print_error("memory allocation failed for process IDs");
		return (0);
	}
	if (!open_sems(rules))
	{
		free(rules->pids);
		rules->pids = NULL;
		return (0);
	}
	return (1);
}
