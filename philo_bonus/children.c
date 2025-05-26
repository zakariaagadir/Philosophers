/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 02:57:59 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_children(t_philo *philos, t_info *infos)
{
	int	i;

	i = 0;
	while (i < (*infos).philo)
	{
		philos[i].last_meal_time = (*infos).start;
		philos[i].thread = fork();
		if (philos[i].thread < 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		if (philos[i].thread == 0)
		{
			philo_routine(&philos[i]);
			exit(0);
		}
		else
			i++;
	}
	return (0);
}

void	waite_for_children(t_philo *philos, t_info *infos, int i,
		int finished_meals)
{
	int		status;
	pid_t	pid;
	int		j;

	while (i < (*infos).philo)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
			{
				j = 0;
				while (j < (*infos).philo)
					kill(philos[j++].thread, SIGKILL);
				break ;
			}
			else if (WEXITSTATUS(status) == 0)
			{
				if (++finished_meals == (*infos).philo)
					break ;
			}
		}
		i++;
	}
}
