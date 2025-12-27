/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:15:02 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:15:04 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '0' && str[1] != '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (i > 10)
		return (0);
	if (i == 10 && str[0] > '2')
		return (0);
	if (i == 10 && str[0] == '2' && str[1] > '1')
		return (0);
	return (1);
}

int	validate_args(int argc, char **argv)
{
	long	val;
	int		i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (1);
		val = ft_atoi(argv[i]);
		if (val > INT_MAX || val <= 0)
			return (1);
		if (i == 1 && (val < 1 || val > 200))
			return (1);
		if ((i >= 2 && i <= 4) && val <= 60)
		{
			handle_error(3, 0);
			return (1);
		}
		if (i == 5 && val <= 0)
			return (1);
		i++;
	}
	return (0);
}
