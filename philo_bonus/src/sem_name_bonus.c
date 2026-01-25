/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_name_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 12:00:00 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/23 16:49:10 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*make_sem_name(char *base)
{
	char	*pid_str;
	char	*time_str;
	char	*tmp;
	char	*name;

	pid_str = ft_itoa((int)getpid());
	time_str = ft_itoa((int)(get_time_ms() % 100000));
	tmp = ft_strjoin(base, "_");
	name = ft_strjoin(tmp, pid_str);
	name = ft_strjoin(name, time_str);
	free(pid_str);
	free(time_str);
	free(tmp);
	return (name);
}
