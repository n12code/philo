/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:34:09 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/03 20:41:34 by nbodin           ###   ########lyon.fr   */
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
	// u_int64_t time =get_time();
	// u_int64_t last_time = get_last_meal_time(philo);
	
	// pthread_mutex_lock(&philo->data->print_lock);
	// printf("time = %llu\n", time);
	// printf("last_time = %llu\n", last_time);
	// printf("time_to_die = %llu\n", philo->data->time_to_die);
	// pthread_mutex_unlock(&philo->data->print_lock);
	if (get_time() - get_last_meal_time(philo) >= philo->data->time_to_die) //&& !get_eating_state(philo)
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
    while (get_philos_state(data) != 1)  // Only check stop condition
    {
        usleep(100);
        if (philo_died(&data->philos[i]))
        {
            print_message(&data->philos[i], DIED);
            change_philos_state(data);
            break;  // Exit after death detection
        }
        i = (i + 1) % data->n_philos;  // Cycle: 0,1,2,3,0,1,2,3...
    }
	pthread_mutex_lock(&data->print_lock);
	printf("got here\n");
	pthread_mutex_unlock(&data->print_lock);
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
        
        // Check all philosophers
        while (i < data->n_philos)
        {
            if (philo_full(&data->philos[i]))
                full_count++;
            i++;
        }
        
        // If all are full, stop simulation
        if (full_count == data->n_philos)
        {
            change_philos_state(data);
            break;
        }
    }
    return (NULL);
}