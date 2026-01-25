/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_error(char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	print_usage(void)
{
	write(2, "Usage: ./philo_bonus N T_DIE T_EAT T_SLEEP [T_EAT_COUNT]\n", 57);
	write(2, "\n  N:              number of philosophers (1-200)\n", 50);
	write(2, "  T_DIE:          time to die in milliseconds (> 60)\n", 53);
	write(2, "  T_EAT:          time to eat in milliseconds (> 60)\n", 53);
	write(2, "  T_SLEEP:        time to sleep in milliseconds (> 60)\n", 55);
	write(2, "  T_EAT_COUNT:    (optional) meals per philosopher\n", 51);
}

int	error_exit(char *msg, int code)
{
	print_error(msg);
	return (code);
}
