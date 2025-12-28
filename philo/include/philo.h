/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:04:42 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/29 02:54:21 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* Standard Library Includes */
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

/* Type Definitions */
typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		meal_lock;
	t_data				*data;
}						t_philo;

struct					s_data
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	long long			start_time;
	bool				dead_flag;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*forks;
	t_philo				*philos;
};

/* Utility Functions (utils.c) */
int						ft_atoi(const char *str);
long long				get_time(void);
void					ft_usleep(long long time, t_data *data);
void					print_msg(char *str, t_philo *philo);
size_t					ft_strlen(const char *str);

/* Argument Validation (validation.c) */
int						is_valid_number(const char *str);
int						validate_args(int argc, char **argv);
int						validate_arg_numeric(const char *arg);
int						validate_arg_range(int i, long val);
int						validate_arg(int i, char *arg);

/* Error Handling (error.c) */
void					ft_putstr_fd(const char *s, int fd);
void					destroy_mutexes(t_data *data);
void					cleanup(t_data *data);

/* Initialization (init.c) */
int						init_data(t_data *data, int argc, char **argv);
int						init_philos(t_data *data);
int						init_forks(t_data *data);

/* Philosopher Routine (routine.c) */
void					*philo_routine(void *pointer);
void					lock_forks(t_philo *philo);
void					eat(t_philo *philo);

/* Monitor & State (monitor.c) */
void					*monitor_routine(void *pointer);
bool					check_dead(t_data *data);
void					set_dead_flag(t_data *data);
int						check_if_dead(t_data *data);
int						check_if_all_ate(t_data *data);

#endif
