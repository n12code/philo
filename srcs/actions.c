/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:12:59 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/11 17:57:27 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	philos_eat(t_philo *philos)
{
	if (take_forks(philos))
		return (1);
	pthread_mutex_lock(&philos->philo_mutex.mutex);
	philos->last_meal = get_time_us();
	philos->eating = 1;
	philos->meal_end_time = get_time_us() / 1000LL + philos->data->time_to_eat;
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->philo_mutex.mutex);
	log_action(philos->data, philos->id, EATING, GREEN);
	sleep_till(philos->meal_end_time);
	drop_forks(philos);
	pthread_mutex_lock(&philos->philo_mutex.mutex);
	philos->eating = 0;
	pthread_mutex_unlock(&philos->philo_mutex.mutex);
	return (get_philos_state(philos->data));
}

int	take_forks(t_philo *philos)
{
	t_mutex	*first_fork;
	t_mutex	*second_fork;
	int		loop_result;

	first_fork = NULL;
	second_fork = NULL;
	loop_result = 0;
	assign_forks(philos, &first_fork, &second_fork);
	if (philos->data->n_philos == 1)
		return (handle_single_philo(philos));
	while (!get_philos_state(philos->data))
	{
		loop_result = take_forks_loop(philos, &first_fork, &second_fork);
		if (loop_result == 1 || loop_result == 0)
			return (loop_result);
		usleep(FORK_RETRY_DELAY);
	}
	try_to_release_forks(philos, &first_fork, &second_fork);
	return (1);
}

int	philos_think(t_philo *philos)
{
	long long	think_time;

	if (get_philos_state(philos->data))
		return (1);
	if (log_action(philos->data, philos->id, THINKING, CYAN))
		return (1);
	think_time = (philos->data->time_to_eat - philos->data->time_to_sleep) / 2;
	if (think_time <= 0)
		think_time = 1;
	ft_usleep_precise(think_time * 1000, philos->data);
	return (0);
}

int	philos_sleep(t_philo *philos)
{
	long long	wake_up_time;

	if (get_philos_state(philos->data))
		return (1);
	if (log_action(philos->data, philos->id, SLEEPING, BLUE))
		return (1);
	pthread_mutex_lock(&philos->philo_mutex.mutex);
	wake_up_time = get_time() + philos->data->time_to_sleep;
	philos->wake_up_time = wake_up_time;
	pthread_mutex_unlock(&philos->philo_mutex.mutex);
	sleep_till(wake_up_time);
	return (0);
}
