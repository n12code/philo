/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:24:47 by nbodin            #+#    #+#             */
/*   Updated: 2025/06/03 15:01:53 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	if (ft_atol(argv[1]) == -1 || ft_atol(argv[2]) == -1
		|| ft_atol(argv[3]) == -1 || ft_atol(argv[4]) == -1)
		return (1);//overflow
	data->philos_num =  (int) argv[1];
	data->death_time = (u_int64_t) argv[2];
	data->eat_time = (u_int64_t) argv[3];
	data->sleep_time = (u_int64_t)argv[4];
	if (argv[5] && argc == 6)
	{
		if (ft_atol(argv[5]) == -1)
			return (1);//overflow
		data->meals_num = (int) argv[5];
	}
	else
		data->meals_num = -1;
	if (data->philos <= 0 || data->philos > 200)
		return (1);
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->write, NULL);
	data->th_arr = malloc(data->philos_num * sizeof(pthread_t));
	if (!data->th_arr)
		return (1);//malloc
	return (0);
}

int	init_philos(t_data *data)
{
	size_t	i;
	
	i = 0;
	data->philos = malloc(data->philos_num * sizeof(t_philo));
	if (!data->philos)
		retun (1);
	while (i < data->philos_num)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].meal_count = 0;
		data->philos[i].eating = 0;
		data->philos[i].done = 0;
		pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
	}
	return (0);
}

int	init_forks(t_data *data)
{
	size_t	i;

	i = 0;
	data->forks = malloc(data->philos_num * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (1);
	while (i < data->philos_num)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 1;
	data->philos[0].l_fork = &data->forks[0];
	data->philos[0].r_fork = &data->forks[data->philos_num - 1];
	while (i < data->philos_num)
	{
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

int	init(t_data	*data, int argc, char **argv)
{
	if (init_data(data, argc, argv))
		return (1);
	if (init_philos(data))
		return (1);
	if (init_forks(data))
		return (1);
	data->th_arr = malloc(data->philos_num * sizeof(pthread_t));
	if (!data->th_arr)
		return (1);
	return (0);
}