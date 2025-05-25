#include "philo.h"

void      ft_is_number(char *str)
{
  int i;
  i = 0;
  while(str[i])
  {
    if (!ft_isdigit(str[i]))
    {
      write(2, "enter numbers\n", 15);
      exit (1);
    }
    i++;
  }
}

long long timestamp_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void  parcing(int argc, char **argv, t_info *infos)
{
  int i;
  int data[5];

  if (argc > 6 || argc < 5)
    exit(1);
  i = 1;
  while(i < (argc))
  {
    ft_is_number(argv[i]);
    data[i-1]= ft_atoi(argv[i]);
    i++;
  }
  if(argc == 5)
    data[4] = -1;
  infos->number_of_eat = data[4];
  infos->philo = data[0];
  infos->time_to_die = data[1];
  infos->time_to_eat = data[2];
  infos->time_to_sleep = data[3];
  if(infos->philo <= 1)
  {
    if(infos->philo == 1)
        printf("%lld %d died\n", 0LL, 1);
    exit (1);
  }
}

int         check_died(t_philo *philos, int i)
{
    t_info *info = philos[0].info;
    long now = timestamp_ms();

    pthread_mutex_lock(&info->stop_mutex);
    if (now - philos[i].last_meal_time >= info->time_to_die)
    {
        info->stop = 1;
        printf("%lld %d died\n", timestamp_ms() - philos->info->start, philos[i].id);
        pthread_mutex_unlock(&info->stop_mutex);
        return (1);
    }
    pthread_mutex_unlock(&info->stop_mutex);
    return (0);
}

int         check_stop(t_philo *philos)
{
    t_info *info = philos[0].info;
    int i;

    i = 0;
    while (i < info->philo)
    {
        pthread_mutex_lock(&info->stop_mutex);
        if (info->stop)
        {
            pthread_mutex_unlock(&info->stop_mutex);
            return (1);
        }
        pthread_mutex_unlock(&info->stop_mutex);
        if (check_died(philos, i) == 1)
            return (1);

        i++;
    }
    return (0);
}

void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_info *info = philos[0].info;

    while (1)
    {
        pthread_mutex_lock(&info->stop_mutex);
        if (philos[0].meals_eaten == philos[0].info->number_of_eat)
        {
            pthread_mutex_unlock(&info->stop_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&info->stop_mutex);
        if (check_stop(philos) == 1)
            return (NULL);
        usleep(1000);
    }
}

int eating(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->stop_mutex);
    if (philo->info->stop)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(&philo->info->stop_mutex);
        return (1);
    }
    printf("%lld %d has taken a fork\n", timestamp_ms() - philo->info->start, philo->id);

    philo->last_meal_time = timestamp_ms();
    printf("%lld %d is eating\n", timestamp_ms() - philo->info->start, philo->id);
    pthread_mutex_unlock(&philo->info->stop_mutex);
    usleep(philo->info->time_to_eat * 1000);
    pthread_mutex_lock(&philo->info->stop_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->info->stop_mutex);

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    return (0);
}

int sleeping(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->stop_mutex);
    if (philo->info->stop )
    {
        pthread_mutex_unlock(&philo->info->stop_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->info->stop_mutex);
    pthread_mutex_lock(&philo->info->stop_mutex);
    if (philo->info->stop)
    {
        pthread_mutex_unlock(&philo->info->stop_mutex);
        return (1);
    }
    printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start, philo->id);
    pthread_mutex_unlock(&philo->info->stop_mutex);
    usleep(philo->info->time_to_sleep * 1000);
    return (0);
}

int thinking(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->stop_mutex);
    if (philo->info->stop)
    {
        pthread_mutex_unlock(&philo->info->stop_mutex);
        return (1);
    }
    printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start, philo->id);
    pthread_mutex_unlock(&philo->info->stop_mutex);
    usleep (1000);
    pthread_mutex_lock(&philo->info->stop_mutex);
    if (philo->meals_eaten == philo->info->number_of_eat)
    {
        pthread_mutex_unlock(&philo->info->stop_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->info->stop_mutex);
    return (0);
}

void    check_prioritie(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        usleep (1);
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(philo->left_fork);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
    }
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!philo->info->stop)
    {
        pthread_mutex_lock(&philo->info->stop_mutex);
        if (philo->info->stop)
        {
            pthread_mutex_unlock(&philo->info->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->info->stop_mutex);
        check_prioritie(philo);
       
        if (eating(philo) == 1)
            break;
        if (sleeping(philo) == 1)
            break;
        if (thinking(philo) == 1)
            break;
    }
    return NULL;
}

void    initialise(t_philo *philos, t_info *infos, pthread_mutex_t *forks)
{
    int     i;

    i = 0;
    while (i < (*infos).philo)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    i = 0;
    while (i < (*infos).philo)
    {
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].last_meal_time = timestamp_ms();
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % (*infos).philo];
        philos[i].info = infos;
        i++;
    }

}

int    create_threads(t_philo *philos, t_info *infos, pthread_t *monitor_thread)
{
    int     i;

    i = 0;
    while (i < (*infos).philo)
    {
        philos[i].last_meal_time = (*infos).start;
        if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        i++;
    }
    if (pthread_create(monitor_thread, NULL, &monitor, philos) != 0)
    {
        perror("Failed to create monitor thread");
        return 1;
    }
    return (0);
}

void    join_clear(pthread_t monitor_thread, t_info infos, pthread_mutex_t *forks, t_philo *philos)
{
    int i;
    pthread_join(monitor_thread, NULL);
    i = 0;
    while (i < infos.philo)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    i = 0;
    while (i < infos.philo)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_mutex_destroy(&infos.stop_mutex);
    free(philos);
    free(forks);
}

int main(int argc, char **argv)
{
    pthread_mutex_t *forks;
    t_philo         *philos;
    t_info          infos;
    pthread_t       monitor_thread;

    ft_bzero(&infos, sizeof(t_info));
    parcing(argc, argv, &infos);
    infos.stop = 0;
    pthread_mutex_init(&infos.stop_mutex, NULL);
    forks = malloc(sizeof(pthread_mutex_t) * infos.philo);
    if (!forks)
        return 1;
    philos = malloc(sizeof(t_philo) * infos.philo);
    if (!philos)
    {
        free(forks);
        return 1;
    }
    initialise(philos, &infos, forks);
    infos.start = timestamp_ms();
    if (create_threads(philos, &infos, &monitor_thread) == 1)
        return (1);
    join_clear(monitor_thread, infos, forks, philos);
    return 0;
}
