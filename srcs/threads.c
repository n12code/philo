/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:14:34 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/03 20:41:55 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*routine(void *philos_pointer)
{
	t_philo	*philos;

	philos = (t_philo *) philos_pointer;
	pthread_mutex_lock(&philos->data->last_meal_lock);
	philos->last_meal = philos->data->start_time;
	pthread_mutex_unlock(&philos->data->last_meal_lock);
	while (!get_philos_state(philos->data))
	{
		if (philos_eat(philos))
			break ;
		if (get_philos_state(philos->data))
			break ;
		if (philos_sleep(philos))
			break ;
		if (get_philos_state(philos->data))
			break ;
		if (philos_think(philos))
			break ;
	}
	return (NULL);
}

int	create_threads(t_data *data)
{
	size_t	i;
	t_philo	*philos;
	
	i = 0;
	data->start_time = get_time();
	philos = data->philos;
	while (i < data->n_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&data->monitor_alive, NULL, &monitor_alive, &data))
	 	return (1);
	if (data->n_meals != -1)
	{
		if (pthread_create(&data->monitor_full, NULL, &monitor_full, &data))
			return (1);
	}
	return (0);
}

int	join_threads(t_data *data)
{
	size_t	i;
	
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (1);
		i++;
	}
	if (pthread_join(data->monitor_alive, NULL))
		return (1);
	if (data->n_meals != -1)
	{
		if (pthread_join(data->monitor_full, NULL))
			return (1);
	}
	return (0);
}