#ifndef PHILO_H
# define PHILO_H

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
	int			bonus;
	int			time_to_die;
	long long	start;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_eat;
	sem_t		*stop_mutex;
}				t_info;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	int			number_of_eate;
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
					int finished_meals);
int				create_children(t_philo *philos, t_info *infos);
void			initialise_semaphore(t_philo *philos, sem_t **forks,
					t_info *infos);
void			eating(t_philo *philo);
void			waiting(t_philo *philo);
void			parcing(int argc, char **argv, t_info *infos);
long long		timestamp_ms(void);

#endif