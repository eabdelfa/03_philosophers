/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:02:46 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/26 18:17:11 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	alloc_and_init(t_rules *rules)
{
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num);
	rules->philos = malloc(sizeof(t_philo) * rules->num);
	if (!rules->forks || !rules->philos)
	{
		free(rules->forks);
		free(rules->philos);
		rules->forks = NULL;
		rules->philos = NULL;
		return (0);
	}
	if (!init_mutexes(rules))
	{
		cleanup_partial_init(rules);
		free(rules->philos);
		rules->philos = NULL;
		return (0);
	}
	return (1);
}

int	init_rules(t_rules *rules, int ac, char **av)
{
	if (!parse_rules(rules, ac, av))
		return (0);
	if (!alloc_and_init(rules))
	{
		print_error("failed to allocate memory or initialize mutexes");
		return (0);
	}
	return (1);
}
