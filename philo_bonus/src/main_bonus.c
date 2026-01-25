/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:12 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int ac, char **av)
{
	t_rules	rules;

	if (ac != 5 && ac != 6)
	{
		print_usage();
		return (1);
	}
	memset(&rules, 0, sizeof(t_rules));
	if (!init_rules(&rules, ac, av))
	{
		cleanup_rules(&rules);
		print_usage();
		return (1);
	}
	start_simulation(&rules);
	cleanup_rules(&rules);
	return (0);
}
