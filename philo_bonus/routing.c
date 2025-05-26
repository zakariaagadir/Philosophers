/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 02:58:21 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void	*routine_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		waiting(philo);
		eating(philo);
		sem_wait(philo->info->stop_mutex);
		printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start,
			philo->id);
		sem_post(philo->info->stop_mutex);
		usleep(philo->info->time_to_sleep * 1000);
		sem_wait(philo->info->stop_mutex);
		printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start,
			philo->id);
		sem_post(philo->info->stop_mutex);
		usleep(1000);
		sem_wait(philo->info->stop_mutex);
		if (philo->meals_eaten == philo->info->number_of_eat)
		{
			sem_post(philo->info->stop_mutex);
			exit(0);
		}
		sem_post(philo->info->stop_mutex);
	}
	return (NULL);
}

void	eating(t_philo *philo)
{
	long long	now;

	sem_wait(philo->info->stop_mutex);
	now = timestamp_ms();
	philo->last_meal_time = now;
	printf("%lld %d has taken a fork\n", now - philo->info->start, philo->id);
	printf("%lld %d is eating\n", now - philo->info->start, philo->id);
	sem_post(philo->info->stop_mutex);
	usleep(philo->info->time_to_eat * 1000);
	sem_post(philo->left_fork);
	sem_post(philo->right_fork);
	sem_wait(philo->info->stop_mutex);
	philo->meals_eaten++;
	sem_post(philo->info->stop_mutex);
}

void	waiting(t_philo *philo)
{
	sem_wait(philo->info->stop_mutex);
	if (philo->meals_eaten == philo->info->number_of_eat)
	{
		sem_post(philo->info->stop_mutex);
		exit(0);
	}
	sem_post(philo->info->stop_mutex);
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
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	monitor;

	philo = (t_philo *)arg;
	if (pthread_create(&monitor, NULL, monitor_thread, philo) != 0)
	{
		perror("Failed to create monitor thread");
		exit(1);
	}
	routine_thread(philo);
	pthread_join(monitor, NULL);
	return (NULL);
}

void	*monitor_thread(void *arg)
{
	t_philo		*philo;
	long long	now;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->info->stop_mutex);
		now = timestamp_ms();
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
	return (NULL);
}
