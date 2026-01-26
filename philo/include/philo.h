/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:03:17 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/26 19:38:08 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int					id;
	int					meals;
	long				last_meal;
	pthread_t			thread;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		*left;
	pthread_mutex_t		*right;
	t_rules				*rules;
}						t_philo;

struct					s_rules
{
	int					num;
	long				t_die;
	long				t_eat;
	long				t_sleep;
	int					must_eat;
	int					has_must;
	long				start;
	int					stop;
	int					stop_init;
	int					print_init;
	int					forks_init;
	int					meals_init;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*forks;
	t_philo				*philos;
};

int						init_rules(t_rules *rules, int ac, char **av);
int						parse_rules(t_rules *rules, int ac, char **av);
int						init_philos(t_rules *rules);
int						init_mutexes(t_rules *rules);
void					cleanup_partial_init(t_rules *rules);
int						start_simulation(t_rules *rules);
void					cleanup_rules(t_rules *rules);

void					*philo_routine(void *arg);
void					take_forks(t_philo *philo);
void					release_forks(t_philo *philo);
void					eat(t_philo *philo);
void					update_meal_time(t_philo *philo, long now);
void					increment_meals(t_philo *philo);
void					monitor_simulation(t_rules *rules);

long					get_time_ms(void);
void					ft_usleep(t_rules *rules, long ms);

int						is_positive_number(char *s);
long					ft_atol(char *s);
size_t					ft_strlen(char *s);

int						get_stop(t_rules *rules);
void					set_stop(t_rules *rules);
void					print_state(t_philo *philo, char *msg, int force);

void					print_error(char *msg);
void					print_usage(void);
int						error_exit(char *msg, int code);

#endif
