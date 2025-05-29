/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 13:39:50 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*routine_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->info->stop_mutex);
		if (philo->info->die == 1
			|| philo->meals_eaten == philo->info->number_of_eat)
			return (sstop(philo));
		sem_post(philo->info->stop_mutex);
		eating_waiting(philo);
		sem_wait(philo->info->stop_mutex);
		if (philo->info->die == 1)
			return (sstop(philo));
		printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start,
			philo->id);
		sem_post(philo->info->stop_mutex);
		ft_usleep(philo->info->time_to_sleep);
		sem_wait(philo->info->stop_mutex);
		if (philo->info->die == 1)
			return (sstop(philo));
		sleep_post(philo);
	}
	return (NULL);
}

void	eating(t_philo *philo)
{
	long long	now;

	sem_wait(philo->info->stop_mutex);
	if (philo->info->die == 1)
	{
		sem_post(philo->info->stop_mutex);
		return ;
	}
	now = timestamp_ms();
	philo->last_meal_time = now;
	printf("%lld %d has taken a fork\n", now - philo->info->start, philo->id);
	printf("%lld %d is eating\n", now - philo->info->start, philo->id);
	sem_post(philo->info->stop_mutex);
	ft_usleep(philo->info->time_to_eat);
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
		ft_usleep(1);
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
		write(2, "Failed to create monitor thread", 32);
		exit(1);
	}
	routine_thread(philo);
	pthread_join(monitor, NULL);
	sem_close(philo->info->stop_m);
	sem_unlink("/stop_s");
	sem_close(philo->info->stop_mutex);
	sem_unlink("/stop_sem");
	sem_close(philo->right_fork);
	sem_unlink("/fork_");
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
		if (philo->info->stop_m->__align == 0)
		{
			philo->info->die = 1;
			sem_post(philo->info->stop_mutex);
			return (NULL);
		}
		sem_post(philo->info->stop_mutex);
		sem_wait(philo->info->stop_mutex);
		now = timestamp_ms();
		if (now - philo->last_meal_time > philo->info->time_to_die)
			return (diiie(philo, now));
		if (philo->meals_eaten == philo->info->number_of_eat)
			return (sstop(philo));
		sem_post(philo->info->stop_mutex);
		ft_usleep(1);
	}
	return (NULL);
}
