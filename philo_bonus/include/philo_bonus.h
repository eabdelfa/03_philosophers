/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:05:22 by eabdelfa          #+#    #+#             */
/*   Updated: 2025/12/28 18:31:27 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* Standard Library Includes */
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

/* Type Definitions */
typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal_time;
	pid_t				pid;
	sem_t				*meal_sem;
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
	sem_t				*sem_forks;
	sem_t				*sem_write;
	t_philo				*philos;
};

/* Utility Functions (utils_bonus.c) */
int						ft_atoi(const char *str);
long long				get_time(void);
void					ft_usleep(long long time);
void					print_msg(char *str, t_philo *philo);
int						get_len(int n);

/* Argument Validation (validation_bonus.c) */
int						is_valid_number(const char *str);
int						validate_args(int argc, char **argv);
int						validate_arg_numeric(const char *str);
int						validate_arg_range(int i, long val);
int						validate_arg(int i, char *arg);

/* Error Handling (error_bonus.c) */
void					print_error_and_exit(const char *msg);
void					ft_putstr_fd(const char *s, int fd);

/* Initialization (init_bonus.c) */
int						init_data(t_data *data, int argc, char **argv);
int						init_semaphores(t_data *data);
void					cleanup(t_data *data);
void					make_sem_name(char *buffer, int id);

/* Philosopher Routine (routine_bonus.c) */
void					philo_process(t_philo *philo);
void					*monitor_routine(void *philo_ptr);

#endif
