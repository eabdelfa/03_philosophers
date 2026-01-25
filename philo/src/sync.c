/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_stop(t_rules *rules)
{
	int	stop;

	pthread_mutex_lock(&rules->stop_mutex);
	stop = rules->stop;
	pthread_mutex_unlock(&rules->stop_mutex);
	return (stop);
}

void	set_stop(t_rules *rules)
{
	pthread_mutex_lock(&rules->stop_mutex);
	rules->stop = 1;
	pthread_mutex_unlock(&rules->stop_mutex);
}
