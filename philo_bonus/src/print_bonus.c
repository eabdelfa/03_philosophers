/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_state(t_philo *philo, char *msg)
{
	long	time;

	sem_wait(philo->rules->print);
	time = get_time_ms() - philo->rules->start;
	printf("%ld %d %s\n", time, philo->id, msg);
	sem_post(philo->rules->print);
}
