/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:14:26 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 23:08:36 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** is_valid_number:
** Checks if a string is a valid positive integer (no leading zeros,
	max 10 digits, within range).
*/
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

/*
** validate_arg_numeric:
** Checks if the argument is a valid positive integer and in range.
*/
int	validate_arg_numeric(const char *arg)
{
	long	val;

	if (!is_valid_number(arg))
	{
		ft_putstr_fd("Error: Not a valid positive number.\n", 2);
		return (1);
	}
	val = ft_atoi(arg);
	if (val > INT_MAX || val <= 0)
	{
		ft_putstr_fd("Error: Argument out of integer range.\n", 2);
		return (1);
	}
	return (0);
}

/*
** validate_arg_range:
** Checks if the argument value is within allowed ranges for its position.
*/
int	validate_arg_range(int i, long val)
{
	if (i == 1 && (val < 1 || val > 200))
	{
		ft_putstr_fd("Error: Philosophers must be 1 to 200.\n", 2);
		return (1);
	}
	if ((i >= 2 && i <= 4) && val <= 60)
	{
		print_error_and_exit("Error: Time args must be > 60 ms.\n");
	}
	if (i == 5 && val <= 0)
	{
		ft_putstr_fd("Error: Must eat count must be positive.\n", 2);
		return (1);
	}
	return (0);
}

/*
** validate_arg:
** Validates a single command-line argument by numeric and range checks.
*/
int	validate_arg(int i, char *arg)
{
	long	val;

	if (validate_arg_numeric(arg))
		return (1);
	val = ft_atoi(arg);
	return (validate_arg_range(i, val));
}

/*
** validate_args:
** Validates all command-line arguments for the philosophers program.
*/
int	validate_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (validate_arg(i, argv[i]))
			return (1);
		i++;
	}
	return (0);
}
