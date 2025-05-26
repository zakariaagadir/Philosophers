/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 03:07:39 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_is_number(char *str)
{
	int	i;

	i = 0;
	if (str && str[i] == '+')
		i++;
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

long long	timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	parcing(int argc, char **argv, t_info *infos)
{
	int	i;
	int	data[5];

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
	if (infos->philo <= 1 || infos->philo > 200)
	{
		if (infos->philo == 1)
			printf("%lld %d died\n", 0LL, 1);
		exit(1);
	}
}

void	initialise_semaphore(t_philo *philos, sem_t **forks, t_info *infos)
{
	int	i;

	sem_unlink("/stop_sem");
	(*infos).stop_mutex = sem_open("/stop_sem", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/fork_");
	*forks = sem_open("/fork_", O_CREAT | O_EXCL, 0644, (*infos).philo);
	i = 0;
	while (i < (*infos).philo)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = timestamp_ms();
		philos[i].left_fork = *forks;
		philos[i].right_fork = *forks;
		philos[i].info = infos;
		i++;
	}
}

int	main(int argc, char **argv)
{
	sem_t	*forks;
	t_philo	*philos;
	t_info	infos;

	ft_bzero(&infos, sizeof(t_info));
	parcing(argc, argv, &infos);
	philos = malloc(sizeof(t_philo) * infos.philo);
	if (!philos)
		return (1);
	forks = NULL;
	initialise_semaphore(philos, &forks, &infos);
	infos.start = timestamp_ms();
	if (create_children(philos, &infos) == 1)
		return (1);
	waite_for_children(philos, &infos, 0, 0);
	sem_post(infos.stop_mutex);
	sem_close(forks);
	sem_unlink("/fork_");
	sem_close(infos.stop_mutex);
	sem_unlink("/stop_sem");
	free(philos);
	philos = NULL;
	return (0);
}
