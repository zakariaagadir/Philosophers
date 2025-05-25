#include "philo.h"


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
        // Fork acquisition pattern to avoid deadlocks
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

        // Protect shared data and update timestamp before eating
        sem_wait(philo->info->stop_mutex);
        long long now = timestamp_ms();
        philo->last_meal_time = now;
        printf("%lld %d has taken a fork\n", now - philo->info->start, philo->id);
        printf("%lld %d is eating\n", now - philo->info->start, philo->id);
        sem_post(philo->info->stop_mutex);

        usleep(philo->info->time_to_eat * 1000); // simulate eating

        // Update meals eaten

        // Release forks
        sem_post(philo->left_fork);
        sem_post(philo->right_fork);
        sem_wait(philo->info->stop_mutex);
        philo->meals_eaten++;
        sem_post(philo->info->stop_mutex);
        // Sleeping
        sem_wait(philo->info->stop_mutex);
        printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start, philo->id);
        sem_post(philo->info->stop_mutex);
        usleep(philo->info->time_to_sleep * 1000);

        // Thinking
        sem_wait(philo->info->stop_mutex);
        printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start, philo->id);
        sem_post(philo->info->stop_mutex);

        usleep(1000); // small pause to reduce CPU usage
    }

    return NULL;
}


void *monitor_thread(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        sem_wait(philo->info->stop_mutex);
        long long now = timestamp_ms();
        if (now - philo->last_meal_time > philo->info->time_to_die)
        {
            printf("%lld %d died\n", now - philo->info->start, philo->id);
            exit(1);
            sem_post(philo->info->stop_mutex);
        }
        if (philo->meals_eaten == philo->info->number_of_eat)
        {
            sem_post(philo->info->stop_mutex);
            exit(0);
        }
        sem_post(philo->info->stop_mutex);
        usleep(1000);
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
    int finished_meals;
    pid_t    pid;

    i = 0;
    finished_meals = 0;
    ft_bzero(&infos, sizeof(t_info));
    parcing(argc, argv, &infos);


    forks = malloc(sizeof(sem_t *) * infos.philo);
    if (!forks)
        return 1;
    philos = malloc(sizeof(t_philo) * infos.philo);
    if (!philos)
    {
        free(forks);
        return 1;
    }
    sem_unlink("/stop_sem");
    infos.stop_mutex = sem_open("/stop_sem", O_CREAT | O_EXCL, 0644, 1);
    while (i < infos.philo)
    {
        tmp = ft_itoa(i);
        sem_name = ft_strjoin("/fork_", tmp);
        free(tmp);
        tmp = NULL;
        sem_unlink(sem_name);
        forks[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
        free (sem_name);
        sem_name = NULL;
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
    i = 0;

    while (i < infos.philo)
    {
        pid = waitpid(-1, &status, 0);
        if (pid == -1)
            break;

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 1)
            {
                int j = 0;
                while (j < infos.philo)
                    kill(philos[j++].thread, SIGKILL);

                break;
            }
            else if (WEXITSTATUS(status) == 0)
            {
                finished_meals++;
                if (finished_meals == infos.philo)
                    break;
            }
        }

        i++;
    }
    sem_post(infos.stop_mutex);
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
    sem_unlink("/stop_sem");
    free(philos);
    philos = NULL;
    free(forks);
    forks = NULL;
    return 0;
}

