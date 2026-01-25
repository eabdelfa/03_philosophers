/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/25 21:24:20 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	validate_args(char **av)
{
	if (!is_positive_number(av[1]) || !is_positive_number(av[2])
		|| !is_positive_number(av[3]) || !is_positive_number(av[4]))
	{
		print_error("all arguments must be positive numbers");
		return (0);
	}
	return (1);
}

static int	parse_and_set_timings(t_rules *rules, char **av)
{
	long	num_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;

	num_philos = ft_atol(av[1]);
	time_to_die = ft_atol(av[2]);
	time_to_eat = ft_atol(av[3]);
	time_to_sleep = ft_atol(av[4]);
	if (num_philos == LONG_MAX || time_to_die == LONG_MAX
		|| time_to_eat == LONG_MAX || time_to_sleep == LONG_MAX
		|| num_philos > INT_MAX || num_philos > 200 || num_philos < 1)
	{
		print_error("number of philosophers must be between 1 and 200");
		return (0);
	}
	rules->num = (int)num_philos;
	rules->t_die = time_to_die;
	rules->t_eat = time_to_eat;
	rules->t_sleep = time_to_sleep;
	rules->start = -1;
	return (1);
}

static int	parse_must_eat(t_rules *rules, int ac, char **av)
{
	long	must_eat_count;

	if (ac == 6)
	{
		if (!is_positive_number(av[5]))
		{
			print_error("number must be a positive number");
			return (0);
		}
		must_eat_count = ft_atol(av[5]);
		if (must_eat_count == LONG_MAX || must_eat_count > INT_MAX)
		{
			print_error("number out of range");
			return (0);
		}
		rules->must_eat = (int)must_eat_count;
		rules->has_must = 1;
	}
	return (1);
}

int	parse_rules(t_rules *rules, int ac, char **av)
{
	if (!validate_args(av))
		return (0);
	if (!parse_and_set_timings(rules, av))
		return (0);
	if (!parse_must_eat(rules, ac, av))
		return (0);
	if (rules->num <= 0 || rules->t_die <= 0 || rules->t_eat <= 0
		|| rules->t_sleep <= 0)
	{
		print_error("all time values must be greater than 0");
		return (0);
	}
	return (1);
}
