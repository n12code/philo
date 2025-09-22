/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:17:33 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/21 17:05:15 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	dinner_is_over(t_data *data)
{
	size_t	i;
	int		meals_needed;

	i = 0;
	meals_needed = data->n_meals;
	if (meals_needed == -1)
		return (0);
	while (i < data->n_philos)
	{
		if (get_meals_eaten(&data->philos[i]) < meals_needed)
			return (0);
		i++;
	}
	return (1);
}

void	change_philos_state(t_data *data)
{
	pthread_mutex_lock(&data->stop_lock.mutex);
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_lock.mutex);
}

int	handle_single_philo(t_philo *philos)
{
	try_take_fork(philos->left_fork, philos->id);
	log_action(philos->data, philos->id, FORK, YELLOW);
	while (!get_philos_state(philos->data))
		usleep(1000);
	release_fork(philos->left_fork);
	change_philos_state(philos->data);
	return (1);
}

int	get_nbr_chunks(int num_philosophers)
{
	int	chunks;

	chunks = num_philosophers / CHUNK_SIZE;
	if (num_philosophers % CHUNK_SIZE != 0)
		chunks++;
	return (chunks);
}

void	destroy_mutex_safely(t_mutex *mutex)
{
	if (mutex->init == 1)
		pthread_mutex_destroy(&mutex->mutex);
}
