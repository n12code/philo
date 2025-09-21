/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:18 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/21 16:41:01 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_philos_state(t_data *data)
{
	int	philos_state;

	pthread_mutex_lock(&data->stop_lock.mutex);
	philos_state = data->stop;
	pthread_mutex_unlock(&data->stop_lock.mutex);
	return (philos_state);
}

long long	get_last_meal_time(t_philo *philo)
{
	long long	last_meal;

	pthread_mutex_lock(&philo->philo_mutex.mutex);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->philo_mutex.mutex);
	return (last_meal);
}

int	get_eating_state(t_philo *philo)
{
	int	eating_state;

	pthread_mutex_lock(&philo->philo_mutex.mutex);
	eating_state = philo->eating;
	pthread_mutex_unlock(&philo->philo_mutex.mutex);
	return (eating_state);
}

int	get_meals_eaten(t_philo *philo)
{
	int	meals_eaten;

	pthread_mutex_lock(&philo->philo_mutex.mutex);
	meals_eaten = philo->meals_eaten;
	pthread_mutex_unlock(&philo->philo_mutex.mutex);
	return (meals_eaten);
}

long long	get_time(void)
{
	return (get_time_us() / 1000LL);
}
