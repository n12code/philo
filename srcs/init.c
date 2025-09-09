/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:51 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/08 19:31:55 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			while (i >= 0)
			{
				pthread_mutex_destroy(&data->forks[i].mutex);
				i--;
			}
			return (1);
		}
		data->forks[i].status = 0;
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
	t_philo	*philos;

	i = 0;
	philos = data->philos;
	data->start_time = get_time_us();
	while (i < data->n_philos)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].eating = 0;
		philos[i].meal_end_time = 0;
		philos[i].wake_up_time = 0;
		pthread_mutex_init(&philos[i].philo_mutex, NULL);
		i++;
	}
	return (0);
}

int	init_malloc(t_data *data)
{
	data->forks = malloc(data->n_philos * sizeof(t_mutex));
	if (!data->forks)
		return (1);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	if (!data->philos)
		return (1);
	data->nbr_monitors = get_nbr_chunks(data->n_philos);
	data->monitors = malloc(data->nbr_monitors * sizeof(pthread_t));
	if (!data->monitors)
		return (1);
	return (0);
}

int	init_data(int argc, char **argv, t_data *data)
{
	data->n_philos = (size_t)ft_atoi(argv[1]);
	data->time_to_die = (long long)ft_atoi(argv[2]);
	data->time_to_eat = (long long)ft_atoi(argv[3]);
	data->time_to_sleep = (long long)ft_atoi(argv[4]);
	data->n_meals = -1;
	if (argc == 6)
		data->n_meals = ft_atoi(argv[5]);
	data->stop = 0;
	data->log_lst = NULL;
	data->monitor_data = NULL;
	data->i_monitors = 0;
	data->i_comp_monitor = 0;
	data->i_scribe = 0;
	data->i_philos = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->last_meal_lock, NULL);
	pthread_mutex_init(&data->meals_eaten_lock, NULL);
	pthread_mutex_init(&data->stop_lock, NULL);
	pthread_mutex_init(&data->eating_lock, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	if (init_malloc(data))
		return (1);
	return (0);
}