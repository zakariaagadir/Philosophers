/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing3_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 06:17:27 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 13:40:28 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*diiie(t_philo *philo, long long now)
{
	printf("%lld %d died\n", now - philo->info->start, philo->id);
	if (philo->info->philo == 1)
		sem_post(philo->right_fork);
	philo->info->stop_m->__align = 0;
	philo->info->die = 1;
	sem_post(philo->info->stop_mutex);
	return (NULL);
}

void	*sstop(t_philo *philo)
{
	sem_post(philo->info->stop_mutex);
	return (NULL);
}

void	ft_printf(t_info *infos)
{
	printf("%lld %d died\n", 0LL, 1);
	ft_usleep(infos->time_to_die);
	exit(0);
}

void	sleep_post(t_philo *philo)
{
	printf("%lld %d is thinking\n", timestamp_ms() - philo->info->start,
		philo->id);
	sem_post(philo->info->stop_mutex);
	ft_usleep(1);
}
