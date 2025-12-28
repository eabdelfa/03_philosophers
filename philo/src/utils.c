/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:56 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/28 21:51:03 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
** Sleeps for the given time in ms, checking for death status periodically.
*/
void	ft_usleep(long long time, t_data *data)
{
	long long	start;
	long long	elapsed;
	long long	remain;

	elapsed = 0;
	start = get_time();
	while (1)
	{
		elapsed = get_time() - start;
		if (elapsed >= time)
			break ;
		{
			if (check_dead(data))
				break ;
			remain = time - elapsed;
			if (remain > 5)
				usleep(remain * 500);
			else
				usleep(100);
		}
	}
}

/*
** print_msg:
** Prints a status message for a philosopher in a thread-safe way.
*/
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

/*
** ft_strlen:
** Returns the length of a string.
*/
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}
