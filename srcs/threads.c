/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:14:34 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/04 10:11:57 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*routine(void *philos_pointer)
{
	t_philo	*philos;

	philos = (t_philo *) philos_pointer;
	if (philos->data->n_philos > 50)
    {
        if (philos->id % 2 == 0)
            usleep(100);
    }
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
	philos = data->philos;
	while (i < data->n_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&data->monitor_alive, NULL, &monitor_alive, data))
	 	return (1);
	if (data->n_meals != -1)
	{
		if (pthread_create(&data->monitor_full, NULL, &monitor_full, data))
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