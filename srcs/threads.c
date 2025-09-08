/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:19:32 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/08 19:12:15 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *philos_pointer)
{
    t_philo *philos;

    philos = (t_philo *) philos_pointer;
    if (philos->id % 2 == 0)
        usleep(100);
    while (!get_philos_state(philos->data))
    {
        if (philos->id % 2 == 1)
        {
            if (philos_think(philos))
                break;
            if (get_philos_state(philos->data))
                break;
        }
        if (philos_eat(philos))
            break;
        if (get_philos_state(philos->data))
            break;
        if (philos_sleep(philos))
            break;
        if (get_philos_state(philos->data))
            break;
        if (philos_think(philos))
            break;
    }
    return (NULL);
}

int create_threads(t_data *data)
{
    size_t i;
    t_philo *philos;
    t_monitoring_data *monitor_data;
    
    i = 0;
    philos = data->philos;
    monitor_data = malloc(data->nbr_monitors * sizeof(t_monitoring_data));
    if (!monitor_data)
        return (1);
    data->monitor_data = monitor_data;
    i = 0;
    while (i < (size_t) data->nbr_monitors)
    {
        monitor_data[i].id = i;
        monitor_data[i].nbr_monitors = data->nbr_monitors;
        monitor_data[i].data = data;
        i++;
    }
    i = 0;
    while (i < (size_t) data->n_philos)
    {
        if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]))
		{
			change_philos_state(data);
			data->i_philos = i;
            return (1);
		}
        i++;
    }
	data->i_philos = data->n_philos;
    i = 0;
    while (i < (size_t) data->nbr_monitors)
    {
        if (pthread_create(&data->monitors[i], NULL, &monitor_routine, &monitor_data[i]))
        {
			change_philos_state(data);
			data->i_monitors = i;
            return (1);
		}
        i++;
	}
	data->i_monitors = data->nbr_monitors;
	if (data->n_meals != -1)
    {
        if (pthread_create(&data->completion_monitor, NULL, &completion_monitor_routine, data))
		{
			change_philos_state(data);
            return (1);
		}
    }
	data->i_comp_monitor = 1;
    if (pthread_create(&data->scribe, NULL, &scribe_routine, data))
	{
		change_philos_state(data);
        return (1);
	}
	data->i_scribe = 1;
    return (0);
}

int join_threads(t_data *data)
{
    size_t i;
    
    i = 0;
    while (i < (size_t) data->i_philos)
	{
        if (pthread_join(data->philos[i].thread, NULL))
            return (1);
        i++;
    }
    i = 0;
    while (i < (size_t) data->i_monitors)
    {
        if (pthread_join(data->monitors[i], NULL))
            return (1);
        i++;
	}
	if (data->n_meals != -1 && data->i_comp_monitor != 0)
    {
        if (pthread_join(data->completion_monitor, NULL))
            return (1);
    }
	if (data->i_scribe != 0)
	{
		if (pthread_join(data->scribe, NULL))
			return (1);
	}
    return (0);
}