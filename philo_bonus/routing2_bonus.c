/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing2_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/27 03:46:47 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eating_waiting(t_philo *philo)
{
	waiting(philo);
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
