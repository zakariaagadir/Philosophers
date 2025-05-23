#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
int ft_atoi(const char *str);
void error_atoi();
int ft_isdigit(int c);
void ft_bzero(void *s, size_t n);

typedef struct s_info
{
    int philo;
    int time_to_die;
    long long start;
    int time_to_eat;
    int time_to_sleep;
    int number_of_eat;
    // int         stop;
    sem_t *stop_mutex;
} t_info;

typedef struct s_philo
{
    int id;
    int meals_eaten;
    int number_of_eate;
    long long last_meal_time;
    pid_t thread;
    sem_t *left_fork;
    sem_t *right_fork;
    t_info *info;
} t_philo;

char *ft_strjoin(char const *s1, char const *s2);
int ft_atoi(const char *str);
char *ft_itoa(int n);
size_t ft_strlen(const char *s);
void *monitor_thread(void *arg);
void *routine_thread(void *arg);
void *philo_routine(void *arg);
void *monitor(void *arg);

#endif