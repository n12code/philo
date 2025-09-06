/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:15:40 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/04 10:11:15 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_single_philo(t_philo *philos)
{
	pthread_mutex_lock(philos->left_fork);
	print_message(philos, FORK);
	while (!get_philos_state(philos->data))
	{
	}
	pthread_mutex_unlock(philos->left_fork);
	change_philos_state(philos->data);
	return (1);
}

void free_data(t_data *data)
{
	size_t	i;

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
		free(data->philos);
	pthread_mutex_destroy(&data->last_meal_lock);
	pthread_mutex_destroy(&data->meals_eaten_lock);
	pthread_mutex_destroy(&data->stop_lock);
	pthread_mutex_destroy(&data->print_lock);
}

void	ft_usleep(uint64_t sleep_time, t_data *data)
{
	u_int64_t	start;

	start = get_time();
	while (!get_philos_state(data))
	{
		if ((get_time() - start) >= sleep_time)
			break ;
		usleep(10);
	}
}

u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

void	print_message(t_philo *philos, char *msg)
{
	u_int64_t	time;

	time = 0;
    pthread_mutex_lock(&philos->data->print_lock);
    if (!get_philos_state(philos->data))
	{
		time = get_time() - philos->data->start_time;
        printf("%llu %d %s\n", time, philos->id, msg);
	}
    pthread_mutex_unlock(&philos->data->print_lock);
}