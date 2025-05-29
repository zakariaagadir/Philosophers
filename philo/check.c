/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 15:15:56 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_died(t_philo *philos, int i)
{
	t_info	*info;
	long	now;

	info = philos[0].info;
	now = timestamp_ms();
	pthread_mutex_lock(&info->stop_mutex);
	if (now - philos[i].last_meal_time >= info->time_to_die)
	{
		info->stop = 1;
		printf("%lld %d died\n", timestamp_ms() - philos->info->start,
			philos[i].id);
		pthread_mutex_unlock(&info->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&info->stop_mutex);
	return (0);
}

int	check_stop(t_philo *philos)
{
	t_info	*info;
	int		i;

	info = philos[0].info;
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

void	ft_printf(t_info *infos)
{
	printf("%lld %d died\n", 0LL, 1);
	ft_usleep(infos->time_to_die);
	exit(0);
}
