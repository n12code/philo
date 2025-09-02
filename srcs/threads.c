/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:14:34 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/01 16:50:02 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*routine(void *philos_pointer)
{
	t_philo	*philos;

	philos = (t_philo *) philos_pointer;
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

void	*monitor(void *data_pointer)
{
	t_data	*data;

	data = (t_data *) data_pointer;
	(void)data;
	//track if they are alive
	//track if they are full
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
	// if (pthread_create(&data->monitor, NULL, &monitor, &data))
	// 	return (1);
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
	return (0);
}