/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:56 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/27 16:37:30 by eabdelfa         ###   ########.fr       */
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

static int	is_valid_number(const char *str)
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

void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->write_lock);
}

void	handle_error(int code, int extra)
{
	if (code == 1)
		fprintf(stderr,
			"[1] Usage: ./philo number_of_philosophers "
			"time_to_die time_to_eat time_to_sleep\n"
			"[number_of_times_each_philosopher_must_eat]\n");
	else if (code == 2)
		fprintf(stderr, "[2] Error: Invalid argument values.\n");
	else if (code == 3)
		fprintf(stderr,
			"[3] Error: All time arguments must be greater than 60 ms.\n");
	else if (code == 4)
		fprintf(stderr, "[4] Error: Mutex initialization failed.\n");
	else if (code == 5)
		fprintf(stderr, "[5] Error: Memory allocation for forks failed.\n");
	else if (code == 6)
		fprintf(stderr, "[6] Error: Mutex initialization for fork %d failed.\n",
			extra);
	else if (code == 7)
		fprintf(stderr,
			"[7] Error: Memory allocation for philosophers failed.\n");
	else if (code == 8)
		fprintf(stderr, "[8] Error: Mutex initialization for philosopher "
			"%d failed.\n", extra);
	else if (code == 9)
		fprintf(stderr, "[9] Error: Failed to create thread for philosopher "
			"%d.\n", extra);
}
