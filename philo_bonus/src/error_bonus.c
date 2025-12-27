/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:14:41 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:14:42 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	handle_error(int code, int extra)
{
	if (code == 1)
		fprintf(stderr,
				"[1] Usage: ./philo_bonus number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep\n"
				"[number_of_times_each_philosopher_must_eat]\n");
	else if (code == 2)
		fprintf(stderr, "[2] Error: Invalid argument values.\n");
	else if (code == 3)
		fprintf(stderr,
			"[3] Error: All time arguments must be greater than 60 ms.\n");
	else if (code == 4)
		fprintf(stderr, "[4] Error: Semaphore initialization failed.\n");
	else if (code == 5)
		fprintf(stderr,
			"[5] Error: Memory allocation for philosophers failed.\n");
	else if (code == 6)
		fprintf(stderr,
				"[6] Error: Semaphore initialization for philosopher %d "
				"failed.\n",
				extra);
	else if (code == 7)
		fprintf(stderr,
				"[7] Error: Failed to fork process for philosopher "
				"%d.\n",
				extra);
}
