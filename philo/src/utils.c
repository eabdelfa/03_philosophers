/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:56 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 17:15:46 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** ft_usleep:
** Sleeps for 'time' milliseconds.
** Checks for death status periodically to allow immediate termination.
*/
void	ft_usleep(long long time, t_data *data)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		if (check_dead(data))
			break ;
		usleep(100);
	}
}

void	print_msg(char *str, t_philo *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->data->write_lock);
	if (!check_dead(philo->data))
	{
		time = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}
