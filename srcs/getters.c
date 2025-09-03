/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:44:03 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/03 20:41:22 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_philos_state(t_data *data)
{
	int	philos_state;
	
	pthread_mutex_lock(&data->stop_lock);
	philos_state = data->stop;
	pthread_mutex_unlock(&data->stop_lock);
	return (philos_state);	
}

u_int64_t	get_last_meal_time(t_philo *philo)
{
	u_int64_t	last_meal;
	
	pthread_mutex_lock(&philo->data->stop_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (last_meal);	
}

// int	get_eating_state(t_philo *philo)
// {
// 	int	eating_state;
	
// 	pthread_mutex_lock(&philo->data->stop_lock);
// 	eating_state = philo->eating;
// 	pthread_mutex_unlock(&philo->data->stop_lock);
// 	return (eating_state);	
// }

int	get_meals_eaten(t_philo *philo)
{
	int	meals_eaten;
	
	pthread_mutex_lock(&philo->data->stop_lock);
	meals_eaten = philo->meals_eaten;
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (meals_eaten);
}