/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmounji <zmounji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:13:26 by zmounji           #+#    #+#             */
/*   Updated: 2025/05/26 21:21:11 by zmounji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	main(int argc, char **argv)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	t_info			infos;
	pthread_t		monitor_thread;

	ft_bzero(&infos, sizeof(t_info));
	parcing(argc, argv, &infos);
	infos.stop = 0;
	pthread_mutex_init(&infos.stop_mutex, NULL);
	forks = malloc(sizeof(pthread_mutex_t) * infos.philo);
	if (!forks)
		return (1);
	philos = malloc(sizeof(t_philo) * infos.philo);
	if (!philos)
	{
		free(forks);
		return (1);
	}
	initialise(philos, &infos, forks);
	infos.start = timestamp_ms();
	if (create_threads(philos, &infos, &monitor_thread) == 1)
		return (1);
	join_clear(monitor_thread, infos, forks, philos);
	return (0);
}
