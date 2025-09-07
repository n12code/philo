/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:18 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 11:40:09 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_2.h"

int get_philos_state(t_data *data)
{
    int philos_state;
    
    lock_safely(&data->stop_lock);
    philos_state = data->stop;
    unlock_safely(&data->stop_lock);
    return (philos_state);    
}

long long get_last_meal_time(t_philo *philo)
{
    long long last_meal;
    
    lock_safely(&philo->philo_mutex);
    last_meal = philo->last_meal;
    unlock_safely(&philo->philo_mutex);
    return (last_meal);    
}

int get_eating_state(t_philo *philo)
{
    int eating_state;
    
    lock_safely(&philo->philo_mutex);
    eating_state = philo->eating;
    unlock_safely(&philo->philo_mutex);
    return (eating_state);    
}

int get_meals_eaten(t_philo *philo)
{
    int meals_eaten;
    
    lock_safely(&philo->philo_mutex);
    meals_eaten = philo->meals_eaten;
    unlock_safely(&philo->philo_mutex);
    return (meals_eaten);
}