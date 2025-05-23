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

void ft_is_number(char *str)
{
    int i;
    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
        {
            write(2, "enter numbers\n", 15);
            exit(1);
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

void parcing(int argc, char **argv, t_info *infos)
{
    int i;
    int data[5];
    //   int philo;
    //   int time_to_die;
    //   int time_to_eat;
    //   int time_to_sleep;
    //   int number_of_eat;

    if (argc > 6 || argc < 5)
        exit(1);
    i = 1;
    while (i < (argc))
    {
        ft_is_number(argv[i]);
        data[i - 1] = ft_atoi(argv[i]);
        i++;
    }
    if (argc == 5)
        data[4] = -1;
    infos->number_of_eat = data[4];
    infos->philo = data[0];
    infos->time_to_die = data[1];
    infos->time_to_eat = data[2];
    infos->time_to_sleep = data[3];

    if (infos->philo <= 1)
    {
        if (infos->philo == 1)
            printf("%lld %d died\n", 0LL, 1);
        exit(1);
    }
}

void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_info *info = philos[0].info;
    int ret;
    int i;

    while (1)
    {
        i = 0;
        sem_wait(info->stop_mutex);
        if (philos[0].meals_eaten == philos[0].info->number_of_eat)
        {
            sem_post(info->stop_mutex);
            return NULL;
        }
        sem_post(info->stop_mutex);
        while (i < info->philo)
        {
            ret = sem_trywait(info->stop_mutex);
            if (ret == 0)
            {
                sem_post(info->stop_mutex);
                return NULL;
            }
            sem_post(info->stop_mutex);

            long now = timestamp_ms();
            sem_wait(info->stop_mutex);
            if (now - philos[i].last_meal_time >= info->time_to_die)
            {
                info->stop = 1;
                printf("%lld %d died\n", timestamp_ms() - philos->info->start, philos[i].id);
                sem_post(info->stop_mutex);
                return NULL;
            }
            sem_post(info->stop_mutex);
            i++;
        }
        usleep(1000);
    }
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    pthread_t monitor;

    // Start monitor thread for this philosopher
    if (pthread_create(&monitor, NULL, monitor_thread, philo) != 0)
    {
        perror("Failed to create monitor thread");
        exit(1);
    }

    // Run the main philosopher routine
    routine_thread(philo);

    // Wait for monitor to finish before exiting
    pthread_join(monitor, NULL);
    return NULL;
}

void *routine_thread(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        if (philo->id % 2 == 0)
        {
            usleep(1000);
            sem_wait(philo->right_fork);
            sem_wait(philo->left_fork);
        }
        else
        {
            sem_wait(philo->left_fork);
            sem_wait(philo->right_fork);
        }
        printf("%lld %d has taken a fork\n", timestamp_ms() - philo->info->start, philo->id);

        printf("%lld %d is eating\n", philo->last_meal_time - philo->info->start, philo->id);
        usleep(philo->info->time_to_eat * 1000);
        philo->last_meal_time = timestamp_ms();
        philo->meals_eaten++;
        sem_post(philo->left_fork);
        sem_post(philo->right_fork);

        printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start, philo->id);
        usleep(philo->info->time_to_sleep * 1000);

        printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start, philo->id);
        usleep(1000);
    }

    return NULL;
}

void *monitor_thread(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        long long now = timestamp_ms();
        if (now - philo->last_meal_time > philo->info->time_to_die)
        {
            printf("%lld %d died\n", now - philo->info->start, philo->id);
            exit(1); // Exit this philosopher process
        }
        if (philo->meals_eaten == philo->info->number_of_eat)
        {
            exit(1);
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    sem_t **forks;
    t_philo *philos;
    int i;
    char *tmp;
    char *sem_name;
    t_info infos;
    int status;

    i = 0;
    ft_bzero(&infos, sizeof(t_info));
    parcing(argc, argv, &infos);

    infos.stop_mutex = sem_open("/stop_mutex", O_CREAT | O_EXCL, 0644, 0);

    forks = malloc(sizeof(sem_t *) * infos.philo);
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
        tmp = ft_itoa(i);
        sem_name = ft_strjoin("/fork_", tmp);
        free(tmp);
        tmp = NULL;
        forks[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
        i++;
    }

    i = 0;
    while (i < infos.philo)
    {
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].last_meal_time = timestamp_ms(); // Set start time
        philos[i].left_fork = forks[i];
        philos[i].right_fork = forks[(i + 1) % infos.philo];
        philos[i].info = &infos;
        i++;
    }

    i = 0;
    infos.start = timestamp_ms();
    sem_wait(infos.stop_mutex);
    while (i < infos.philo)
    {
        philos[i].last_meal_time = infos.start;
        philos[i].thread = fork();
        if (philos[i].thread < 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        if (philos[i].thread == 0)
        {
            philo_routine(&philos[i]);
            exit(0);
        }
        else
            i++;
    }
    waitpid(-1, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
    {
        i = 0;
        while (i < infos.philo)
            kill(philos[i++].thread, SIGKILL);
        i = 0;
        while (i < infos.philo)
            waitpid(philos[i++].thread, NULL, 0);
    }
    i = 0;
    while (i < infos.philo)
    {
        sem_close(forks[i]);
        tmp = ft_itoa(i);
        sem_name = ft_strjoin("/fork_", tmp);
        free(tmp);
        tmp = NULL;
        sem_unlink(sem_name);
        free(sem_name);
        sem_name = NULL;
        i++;
    }
    sem_close(infos.stop_mutex);
    free(philos);
    i = 0;
    while (i < infos.philo)
        free(forks[i++]);
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

// lock
// if (!stop)
// printf ()
// unvloc