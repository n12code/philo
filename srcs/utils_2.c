/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:06 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 16:29:39 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_2.h"

long long get_time_us(void)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL))
        return (0);
    return ((tv.tv_sec * 1000000LL) + tv.tv_usec);
}

long long get_time(void)
{
    return (get_time_us() / 1000LL);
}

void sleep_till(long long target_time)
{
    long long diff;

    diff = target_time * 1000LL - get_time_us();
    while (diff > 0)
    {
        usleep(diff / 2);
        diff = target_time * 1000LL - get_time_us();
    }
}

void ft_usleep_precise(long long sleep_time_us, t_data *data)
{
    long long start;
    long long target;

    start = get_time_us();
    target = start + sleep_time_us;
    usleep(sleep_time_us - 100);
    while (!get_philos_state(data) && get_time_us() < target)
        ;
}

int handle_single_philo(t_philo *philos)
{
    lock_safely(philos->left_fork);
    log_action(philos->data, philos->id, FORK, YELLOW);
    while (!get_philos_state(philos->data))
        usleep(1000);
    unlock_safely(philos->left_fork);
    change_philos_state(philos->data);
    return (1);
}

void free_data(t_data *data)
{
    size_t i;

    i = 0;
    if (data->forks)
    {
        while (i < data->n_philos)
        {
            pthread_mutex_destroy(&data->forks[i]);
            i++;
        }
        free(data->forks);
    }
    if (data->philos)
    {
        i = 0;
        while (i < data->n_philos)
        {
            pthread_mutex_destroy(&data->philos[i].philo_mutex);
            i++;
        }
        free(data->philos);
    }
    if (data->monitors)
        free(data->monitors);
    if (data->monitor_data)
        free(data->monitor_data);
    pthread_mutex_destroy(&data->last_meal_lock);
    pthread_mutex_destroy(&data->meals_eaten_lock);
    pthread_mutex_destroy(&data->stop_lock);
    pthread_mutex_destroy(&data->print_lock);
    pthread_mutex_destroy(&data->eating_lock);
    pthread_mutex_destroy(&data->log_mutex);
    pthread_mutex_destroy(&data->death_mutex);
    free_log_lst(data->log_lst);
}
