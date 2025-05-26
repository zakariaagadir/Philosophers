/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 02:57:43 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_philo	*philos;
	t_info	*info;

	philos = (t_philo *)arg;
	info = philos[0].info;
	while (1)
	{
		pthread_mutex_lock(&info->stop_mutex);
		if (philos[0].meals_eaten == philos[0].info->number_of_eat)
		{
			pthread_mutex_unlock(&info->stop_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&info->stop_mutex);
		if (check_stop(philos) == 1)
			return (NULL);
		usleep(1000);
	}
}

void	initialise(t_philo *philos, t_info *infos, pthread_mutex_t *forks)
{
	int	i;

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

int	create_threads(t_philo *philos, t_info *infos, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	while (i < (*infos).philo)
	{
		philos[i].last_meal_time = (*infos).start;
		if (pthread_create(&philos[i].thread, NULL, &philo_routine,
				&philos[i]) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	if (pthread_create(monitor_thread, NULL, &monitor, philos) != 0)
	{
		perror("Failed to create monitor thread");
		return (1);
	}
	return (0);
}

void	join_clear(pthread_t monitor_thread, t_info infos,
		pthread_mutex_t *forks, t_philo *philos)
{
	int	i;

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
