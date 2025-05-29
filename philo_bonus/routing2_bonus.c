/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing2_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/29 11:45:59 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eating_waiting(t_philo *philo)
{
	waiting(philo);
	sem_wait(philo->info->stop_mutex);
	if (philo->info->die == 1)
	{
		sem_post(philo->right_fork);
		sem_post(philo->left_fork);
		sem_post(philo->info->stop_mutex);
		return ;
	}
	sem_post(philo->info->stop_mutex);
	eating(philo);
}

void	ft_usleep(long long l)
{
	long long	time1;
	long long	time2;

	time1 = timestamp_ms();
	while (1)
	{
		usleep(1);
		time2 = timestamp_ms();
		if ((time2 - time1) >= l)
			break ;
	}
}
