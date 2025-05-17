#include "philo.h"

// int balance = 0;
// pthread_mutex_t mutex;

// void write_balance(int new_balance)
// {
//   balance = new_balance;
// }

// int read_balance()
// {
//   return balance;
// }

// void* deposit(void *amount)
// {
//   pthread_mutex_lock(&mutex);

//   int account_balance = read_balance();

//   account_balance += *((int *) amount);

//   write_balance(account_balance);

//   pthread_mutex_unlock(&mutex);

//   return NULL;
// }

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
  int philo;
  int time_to_die;
  int time_to_eat;
  int time_to_sleep;
  int number_of_eat;

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
}


void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_info *info = philos[0].info;
    int i;

    while (1)
    {
        i = 0;
        while (i < info->philo)
        {
            pthread_mutex_lock(&info->stop_mutex);
            if (info->stop)
            {
                pthread_mutex_unlock(&info->stop_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&info->stop_mutex);

            long now = timestamp_ms();
            if (now - philos[i].last_meal_time >= info->time_to_die)
            {
                pthread_mutex_lock(&info->stop_mutex);
                info->stop = 1;
                pthread_mutex_unlock(&info->stop_mutex);
                printf("%lld %d died\n", timestamp_ms(), philos[i].id);
                return NULL;
            }
            i++;
            usleep(1000); // check often
        }
    }
}


void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        pthread_mutex_lock(&philo->info->stop_mutex);
        if (philo->info->stop)
        {
            pthread_mutex_unlock(&philo->info->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->info->stop_mutex);

        printf("%lld %d is thinking\n", timestamp_ms(), philo->id);

        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);

        pthread_mutex_lock(&philo->info->stop_mutex);
        if (philo->info->stop) // Double-check in case someone died while waiting for forks
        {
            pthread_mutex_unlock(&philo->info->stop_mutex);
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            break;
        }
        pthread_mutex_unlock(&philo->info->stop_mutex);

        philo->last_meal_time = timestamp_ms();
        printf("%lld %d is eating\n", timestamp_ms(), philo->id);
        usleep(philo->info->time_to_eat * 1000);
        philo->meals_eaten++;

        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        printf("%lld %d is sleeping\n", timestamp_ms(), philo->id);
        usleep(philo->info->time_to_sleep * 1000);
    }

    return NULL;
}


int main(int argc, char **argv)
{
    pthread_mutex_t *forks;
    t_philo         *philos;
    int             i;
    t_info          infos;
    pthread_t       monitor_thread;

    i = 0;
    ft_bzero(&infos, sizeof(t_info));
    parcing(argc, argv, &infos);

    // Initialize stop flag and mutex
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

    while (i < infos.philo)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }

    i = 0;
    while (i < infos.philo)
    {
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].last_meal_time = timestamp_ms(); // Set start time
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % infos.philo];
        philos[i].info = &infos;
        i++;
    }

    i = 0;
    while (i < infos.philo)
    {
        if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        i++;
    }

    if (pthread_create(&monitor_thread, NULL, monitor, philos) != 0)
    {
        perror("Failed to create monitor thread");
        return 1;
    }

    // Wait for the monitor thread to finish
    pthread_join(monitor_thread, NULL);

    // Now safely wait for all philosophers to finish
    i = 0;
    while (i < infos.philo)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }

    // Clean up
    i = 0;
    while (i < infos.philo)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_mutex_destroy(&infos.stop_mutex);
    free(philos);
    free(forks);

    return 0;
}






// int before = read_balance();
// printf("Before: %d\n", before);

// pthread_t thread1;
// pthread_t thread2;

// pthread_mutex_init(&mutex, NULL);

// int deposit1 = 300;
// int deposit2 = 200;

// pthread_create(&thread1, NULL, deposit, (void*) &deposit1);
// pthread_create(&thread2, NULL, deposit, (void*) &deposit2);

// pthread_join(thread1, NULL);
// pthread_join(thread2, NULL);

// pthread_mutex_destroy(&mutex);

// int after = read_balance();
// printf("After: %d\n", after);