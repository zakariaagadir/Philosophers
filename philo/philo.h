/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 03:43:54 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_info
{
	int				philo;
	int				time_to_die;
	long long		start;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_eat;
	int				stop;
	pthread_mutex_t	stop_mutex;
}					t_info;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				number_of_eate;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_info			*info;
}					t_philo;

int					ft_atoi(const char *str);
void				error_atoi(void);
int					ft_isdigit(int c);
void				ft_bzero(void *s, size_t n);
int					eating(t_philo *philo);
int					sleeping(t_philo *philo);
int					thinking(t_philo *philo);
void				check_prioritie(t_philo *philo);
void				*philo_routine(void *arg);
void				*monitor(void *arg);
void				initialise(t_philo *philos, t_info *infos,
						pthread_mutex_t *forks);
int					create_threads(t_philo *philos, t_info *infos,
						pthread_t *monitor_thread);
void				join_clear(pthread_t monitor_thread, t_info infos,
						pthread_mutex_t *forks, t_philo *philos);
void				join_clear(pthread_t monitor_thread, t_info infos,
						pthread_mutex_t *forks, t_philo *philos);
long long			timestamp_ms(void);
int					check_died(t_philo *philos, int i);
int					check_stop(t_philo *philos);
void				parcing(int argc, char **argv, t_info *infos);

#endif