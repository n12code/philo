/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:20:06 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 14:56:55 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{	
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
		i++;
	}	
 	return (0);
}

int	init_philos(t_data *data)
{
	size_t	i;
	t_philo *philos;

	i = 0;
	philos = data->philos;
	data->start_time = get_time();
	while (i < data->n_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].eating = 0;
		i++;
	}
	return (0);
}

int	init_malloc(t_data *data)
{
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (1);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	if (!data->philos)
		return (1);
	return (0);
}

int init_data(int argc, char **argv, t_data *data)
{
	data->n_philos = (size_t) ft_atoi(argv[1]);
	data->time_to_die = (u_int64_t) ft_atoi(argv[2]);
	data->time_to_eat = (u_int64_t) ft_atoi(argv[3]);
	data->time_to_sleep = (u_int64_t) ft_atoi(argv[4]);
	data->n_meals = -1;
	if (argc == 6)
		data->n_meals = ft_atoi(argv[5]);
	data->stop = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->last_meal_lock, NULL);
	pthread_mutex_init(&data->meals_eaten_lock, NULL);
	pthread_mutex_init(&data->stop_lock, NULL);
	if (init_malloc(data))
		return (1);
	return (0);
}
