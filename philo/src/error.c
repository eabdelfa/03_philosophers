/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:14:05 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/28 18:38:29 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

/*
** ft_putstr_fd:
** Writes a string to the given file descriptor.
*/
void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

/*
** print_error_and_exit:
** Prints an error message to stderr and exits the program.
*/
void	print_error_and_exit(const char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

/*
** destroy_mutexes:
** Destroys the main mutexes in the data structure.
*/
void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->write_lock);
}
