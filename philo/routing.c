/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 15:22:42 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->stop_mutex);
	if (philo->info->stop || philo->meals_eaten == philo->info->number_of_eat)
	{
		if (philo->info->philo != 1)
			pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->info->stop_mutex);
		return (1);
	}
	printf("%lld %d has taken a fork\n", timestamp_ms() - philo->info->start,
		philo->id);
	philo->last_meal_time = timestamp_ms();
	printf("%lld %d is eating\n", timestamp_ms() - philo->info->start,
		philo->id);
	pthread_mutex_unlock(&philo->info->stop_mutex);
	ft_usleep(philo->info->time_to_eat);
	pthread_mutex_lock(&philo->info->stop_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->info->stop_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->stop_mutex);
	if (philo->info->stop)
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
	printf("%lld %d is sleeping\n", timestamp_ms() - philo->info->start,
		philo->id);
	pthread_mutex_unlock(&philo->info->stop_mutex);
	ft_usleep(philo->info->time_to_sleep);
	return (0);
}

int	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->stop_mutex);
	if (philo->info->stop)
	{
		pthread_mutex_unlock(&philo->info->stop_mutex);
		return (1);
	}
	printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start,
		philo->id);
	pthread_mutex_unlock(&philo->info->stop_mutex);
	ft_usleep(1);
	pthread_mutex_lock(&philo->info->stop_mutex);
	if (philo->meals_eaten == philo->info->number_of_eat)
	{
		pthread_mutex_unlock(&philo->info->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->info->stop_mutex);
	return (0);
}

int	check_prioritie(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		ft_usleep(1);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		if (philo->info->philo == 1)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		pthread_mutex_lock(philo->right_fork);
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->info->stop)
	{
		pthread_mutex_lock(&philo->info->stop_mutex);
		if (philo->info->stop
			|| philo->meals_eaten == philo->info->number_of_eat)
		{
			pthread_mutex_unlock(&philo->info->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->info->stop_mutex);
		if (check_prioritie(philo) == 1)
		{
			break ;
		}
		if (eating(philo) == 1)
			break ;
		if (sleeping(philo) == 1)
			break ;
		if (thinking(philo) == 1)
			break ;
	}
	return (NULL);
}
