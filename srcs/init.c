/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:51 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/11 16:52:04 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_monitor_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_monitors)
	{
		data->monitor_data[i].id = i;
		data->monitor_data[i].nbr_monitors = data->nbr_monitors;
		data->monitor_data[i].data = data;
	}
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < (int) data->n_philos)
	{
		if (init_mutex_safely(&data->forks[i]))
			return (1);
		data->forks[i].owner = -1;
		i++;
	}
	i = 0;
	while (i < (int) data->n_philos)
	{
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = data->philos;
	data->start_time = get_time_us();
	while (i < (int) data->n_philos)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].eating = 0;
		philos[i].meal_end_time = 0;
		philos[i].wake_up_time = 0;
		if (init_mutex_safely(&data->philos[i].philo_mutex))
			return (1);
		i++;
	}
	return (0);
}

int	init_mutex_safely(t_mutex *mutex)
{
	mutex->init = 0;
	if (pthread_mutex_init(&mutex->mutex, NULL) == 0)
	{
		mutex->init = 1;
		return (0);
	}
	return (1);
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
	data->i_monitors = 0;
	data->i_comp_monitor = 0;
	data->i_scribe = 0;
	data->i_philos = 0;
	data->nbr_monitors = get_nbr_chunks(data->n_philos);
	if (init_mutex_safely(&data->print_lock)
		|| init_mutex_safely(&data->last_meal_lock)
		|| init_mutex_safely(&data->stop_lock)
		|| init_mutex_safely(&data->meals_eaten_lock)
		|| init_mutex_safely(&data->eating_lock)
		|| init_mutex_safely(&data->log_mutex)
		|| init_mutex_safely(&data->death_mutex))
		return (1);
	return (0);
}
