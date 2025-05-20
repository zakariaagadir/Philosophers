#ifndef PHILO_H
#define PHILO_H


#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
int     ft_atoi(const char *str);
void    error_atoi();
int     ft_isdigit(int c);
void	ft_bzero(void *s, size_t n);



typedef struct s_info{
    int         philo;
    int         time_to_die;
    long long   start;
    int         time_to_eat;
    int         time_to_sleep;
    int         number_of_eat;
    int         stop;
    pthread_mutex_t stop_mutex;
}t_info;


typedef struct s_philo{
    int             id;
    int             meals_eaten;
    int             number_of_eate;
    long long       last_meal_time;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_info          *info;
}t_philo;


#endif