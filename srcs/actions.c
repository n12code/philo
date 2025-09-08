/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:12:59 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/08 19:23:49 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philos_eat(t_philo *philos)
{
    if (take_forks(philos))
        return (1);
    lock_safely(&philos->philo_mutex);
    philos->last_meal = get_time_us();
    philos->eating = 1;
    philos->meal_end_time = get_time_us() / 1000LL + philos->data->time_to_eat;
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
    t_mutex *first_fork;
    t_mutex *second_fork;
    int first_taken = 0;
    int second_taken = 0;
    
    if (philos->data->n_philos == 1)
        return (handle_single_philo(philos));
    
    // Determine fork order to avoid deadlock
    if (philos->left_fork < philos->right_fork)
    {
        first_fork = philos->left_fork;
        second_fork = philos->right_fork;
    } 
    else
    {
        first_fork = philos->right_fork;
        second_fork = philos->left_fork;
    }
    
    // Loop until both forks are acquired or simulation ends
    while (!get_philos_state(philos->data))
    {
        // Try to take first fork
        if (!first_taken && try_take_fork(first_fork))
        {
            first_taken = 1;
            log_action(philos->data, philos->id, FORK, YELLOW);
        }
        
        // If we have the first fork, try to take the second
        if (first_taken && !second_taken && try_take_fork(second_fork))
        {
            second_taken = 1;
            log_action(philos->data, philos->id, FORK, YELLOW);
        }
        
        // If we have both forks, we're done
        if (first_taken && second_taken)
            return (0);
        
        // If simulation is stopping, release any fork we might have
        if (get_philos_state(philos->data))
        {
            if (first_taken)
                release_fork(first_fork);
            if (second_taken)
                release_fork(second_fork);
            return (1);
        }
        
        // Wait before retrying
        usleep(FORK_RETRY_DELAY);
    }
    
    // Clean up if simulation ended
    if (first_taken)
        release_fork(first_fork);
    if (second_taken)
        release_fork(second_fork);
    return (1);
}

void drop_forks(t_philo *philos)
{
    release_fork(philos->right_fork);
    release_fork(philos->left_fork);
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