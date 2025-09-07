/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:12:59 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 16:26:35 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_2.h"

int philos_eat(t_philo *philos)
{
    if (take_forks(philos))
        return (1);
    lock_safely(&philos->philo_mutex);
    philos->last_meal = get_time();
    philos->eating = 1;
    philos->meal_end_time = get_time() + philos->data->time_to_eat;
    philos->meals_eaten++;
    unlock_safely(&philos->philo_mutex);
    log_action(philos->data, philos->id, EATING, GREEN);
    sleep_till(philos->meal_end_time);
    drop_forks(philos);
    lock_safely(&philos->philo_mutex);
    philos->eating = 0;
    unlock_safely(&philos->philo_mutex);
    return (get_philos_state(philos->data));
}

int take_forks(t_philo *philos)
{
    pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;
	
	if (philos->data->n_philos == 1)
		return (handle_single_philo(philos));
    if (philos->id < (philos->id + 1) % (int) philos->data->n_philos)
	{
        first_fork = philos->left_fork;
        second_fork = philos->right_fork;
    } 
	else
	{
        first_fork = philos->right_fork;
        second_fork = philos->left_fork;
    }
    lock_safely(first_fork);
    if (get_philos_state(philos->data))
	{
        unlock_safely(first_fork);
        return (1);
    }
    log_action(philos->data, philos->id, FORK, YELLOW);
    lock_safely(second_fork);
    if (get_philos_state(philos->data))
	{
        unlock_safely(second_fork);
        unlock_safely(first_fork);
        return (1);
    }
    log_action(philos->data, philos->id, FORK, YELLOW);
    return (0);
}

void drop_forks(t_philo *philos)
{
    unlock_safely(philos->right_fork);
    unlock_safely(philos->left_fork);
}

int philos_think(t_philo *philos)
{
    if (get_philos_state(philos->data))
        return (1);
    log_action(philos->data, philos->id, THINKING, CYAN);
    long long think_time = (philos->data->time_to_eat - philos->data->time_to_sleep) / 2;
    if (think_time <= 0)
        think_time = 1;
    ft_usleep_precise(think_time * 1000, philos->data);
    return (0);
}

int philos_sleep(t_philo *philos)
{
    long long wake_up_time;
    
    if (get_philos_state(philos->data))
        return (1);
    log_action(philos->data, philos->id, SLEEPING, BLUE);
    lock_safely(&philos->philo_mutex);
    wake_up_time = get_time() + philos->data->time_to_sleep;
    philos->wake_up_time = wake_up_time;
    unlock_safely(&philos->philo_mutex);
    sleep_till(wake_up_time);
    return (0);
}