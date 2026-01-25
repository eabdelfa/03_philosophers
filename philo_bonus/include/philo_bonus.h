/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelfa <eabdelfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:04:39 by eabdelfa          #+#    #+#             */
/*   Updated: 2026/01/25 21:24:20 by eabdelfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int					id;
	int					meals;
	long				last_meal;
	pthread_t			monitor;
	pthread_mutex_t		meal_mutex;
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
	sem_t				*forks;
	sem_t				*print;
	char				*forks_name;
	char				*print_name;
	pid_t				*pids;
};

int						init_rules(t_rules *rules, int ac, char **av);
int						parse_rules(t_rules *rules, int ac, char **av);
int						open_sems(t_rules *rules);
int						start_simulation(t_rules *rules);
void					cleanup_rules(t_rules *rules);

void					child_process(t_rules *rules, int id);
void					*monitor_routine(void *arg);
void					take_forks(t_philo *philo);
void					release_forks(t_philo *philo);
void					eat(t_philo *philo);
void					update_meal_time(t_philo *philo, long now);
void					increment_meals(t_philo *philo);

long					get_time_ms(void);
void					ft_usleep(long ms);

int						is_positive_number(char *s);
long					ft_atol(char *s);
size_t					ft_strlen(char *s);
char					*ft_strdup(char *s);
char					*ft_strjoin(char *s1, char *s2);
char					*ft_itoa(int n);

void					print_state(t_philo *philo, char *msg);

void					print_error(char *msg);
void					print_usage(void);
int						error_exit(char *msg, int code);
char					*make_sem_name(char *base);

#endif
