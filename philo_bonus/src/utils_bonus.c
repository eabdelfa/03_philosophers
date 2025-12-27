/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:36:36 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 21:31:30 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** get_len:
** Returns the number of digits in an integer (for semaphore name generation).
*/
int	get_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/*
** ft_atoi:
** Converts a string to an integer (handles optional whitespace and sign).
*/
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

/*
** get_time:
** Returns the current time in milliseconds.
*/
long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** ft_usleep:
** Sleeps for the given time in ms (busy-wait), used for timing in simulation.
*/
void	ft_usleep(long long time)
{
	long long	start;
	long long	elapsed;
	long long	remain;

	elapsed = 0;
	start = get_time();
	elapsed = get_time() - start;
	while (elapsed < time)
	{
		remain = time - elapsed;
		if (remain > 5)
			usleep(remain * 500);
		else
			usleep(100);
		elapsed = get_time() - start;
	}
}

/*
** print_msg:
** Prints a status message for a philosopher using a semaphore for output
** synchronization.
*/
void	print_msg(char *str, t_philo *philo)
{
	long long	time;

	sem_wait(philo->data->sem_write);
	time = get_time() - philo->data->start_time;
	printf("%lld %d %s\n", time, philo->id, str);
	sem_post(philo->data->sem_write);
}
