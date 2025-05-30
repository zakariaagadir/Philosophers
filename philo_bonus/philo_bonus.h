/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 13:38:36 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

int				ft_atoi(const char *str);
void			error_atoi(void);
int				ft_isdigit(int c);
void			ft_bzero(void *s, size_t n);

typedef struct s_info
{
	int			philo;
	int			failed;
	int			bonus;
	int			die;
	int			time_to_die;
	long long	start;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_eat;
	sem_t		*stop_mutex;
	sem_t		*stop_m;
}				t_info;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	int			number_of_eate;
	pthread_t	monitor;
	long long	last_meal_time;
	pid_t		thread;
	sem_t		*left_fork;
	sem_t		*right_fork;
	t_info		*info;
}				t_philo;

char			*ft_strjoin(char const *s1, char const *s2);
int				ft_atoi(const char *str);
char			*ft_itoa(int n);
size_t			ft_strlen(const char *s);
void			*monitor_thread(void *arg);
void			*routine_thread(void *arg);
void			*philo_routine(void *arg);
void			*monitor(void *arg);
void			waite_for_children(t_philo *philos, t_info *infos, int i,
					int f);
int				create_children(t_philo *philos, t_info *infos);
void			initialise_semaphore(t_philo *philos, sem_t **forks,
					t_info *infos);
void			eating(t_philo *philo);
void			waiting(t_philo *philo);
void			parcing(int argc, char **argv, t_info *infos);
long long		timestamp_ms(void);
void			eating_waiting(t_philo *philo);
void			ft_usleep(long long l);
void			*diiie(t_philo *philo, long long now);
void			*sstop(t_philo *philo);
void			ft_printf(t_info *infos);
void			sleep_post(t_philo	*philo);

#endif