/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 17:02:44 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	update_meal_time(t_philo *philo, long now)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = now;
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	increment_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	eat(t_philo *philo)
{
	long	now;

	now = get_time_ms();
	update_meal_time(philo, now);
	print_state(philo, "is eating");
	ft_usleep(philo->rules->t_eat);
	increment_meals(philo);
}
