/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:34:09 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 15:21:31 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	change_philos_state(t_data *data)
{
	pthread_mutex_lock(&data->stop_lock);
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_lock);
}

int	philo_died(t_philo *philo)
{
	if (get_time() - get_last_meal_time(philo) >= philo->data->time_to_die && !get_eating_state(philo))
		return (1);
	return (0);
}

int	philo_full(t_philo *philo)
{
	if (get_meals_eaten(philo) >= philo->data->n_meals)
		return (1);
	return (0);
}

void *monitor_alive(void *data_pointer)
{
    t_data *data;
    size_t i;

    data = (t_data *) data_pointer;
    i = 0;
	usleep(1000);
    while (!get_philos_state(data))
    {
        if (philo_died(&data->philos[i]))
        {
            print_message(&data->philos[i], DIED);
            change_philos_state(data);
            break;
        }
        i = (i + 1) % data->n_philos;
		if (i == 0)
			usleep(500);
    }
    return (NULL);
}

void *monitor_full(void *data_pointer)
{
    t_data *data;
    size_t i;
    size_t full_count;

    data = (t_data *) data_pointer;
    while (!get_philos_state(data))
    {
        usleep(100);
        full_count = 0;
        i = 0;
        while (i < data->n_philos)
        {
            if (philo_full(&data->philos[i]))
                full_count++;
            i++;
        }
        if (full_count == data->n_philos)
        {
            change_philos_state(data);
            break;
        }
    }
    return (NULL);
}