/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 22:01:32 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/03 20:40:49 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philos_eat(t_philo *philos)
{
	if (take_forks(philos))
		return (1);
	// pthread_mutex_lock(&philos->data->state_lock);
	// philos->eating = 1;
	// pthread_mutex_unlock(&philos->data->state_lock);
	print_message(philos, EATING);
	pthread_mutex_lock(&philos->data->last_meal_lock);
	philos->last_meal = get_time();
	pthread_mutex_unlock(&philos->data->last_meal_lock);
	pthread_mutex_lock(&philos->data->meals_eaten_lock);
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->data->meals_eaten_lock);
	ft_usleep(philos->data->time_to_eat, philos->data);
	//philos->eating = 0;
	drop_forks(philos);	
	return (get_philos_state(philos->data));
}

int	take_forks(t_philo *philos)
{
	if (get_philos_state(philos->data))
		return (1);
	// if (philos->data->n_philos == 1)
	// 	return (handle_single_philo(philos));
	if (philos->id % 2 == 0)
	{
		pthread_mutex_lock(philos->left_fork);
		print_message(philos, FORK);
		pthread_mutex_lock(philos->right_fork);
		print_message(philos, FORK);
	}
	else
	{
		pthread_mutex_lock(philos->right_fork);
		print_message(philos, FORK);
		pthread_mutex_lock(philos->left_fork);
		print_message(philos, FORK);
	}
	if (get_philos_state(philos->data))
	{
		pthread_mutex_unlock(philos->right_fork);
		pthread_mutex_unlock(philos->left_fork);
		return (1);
	}
	return (0);
}

void	drop_forks(t_philo *philos)
{
	pthread_mutex_unlock(philos->right_fork);
	pthread_mutex_unlock(philos->left_fork);
}

int		philos_think(t_philo *philos)
{
	if (get_philos_state(philos->data))
		return (1);
	print_message(philos, THINKING);
	return (0);
}

int		philos_sleep(t_philo *philos)
{
	if (get_philos_state(philos->data))
		return (1);
	print_message(philos, SLEEPING);
	ft_usleep(philos->data->time_to_sleep, philos->data);
	return (0);
}