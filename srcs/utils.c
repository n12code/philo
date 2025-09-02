/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:15:40 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/02 09:59:38 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_destroy(&data->state_lock);
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

	time = get_time() - philos->data->start_time;
    pthread_mutex_lock(&philos->data->print_lock);
    if (!get_philos_state(philos->data))
        printf("%llu %d %s\n", time, philos->id, msg);
    pthread_mutex_unlock(&philos->data->print_lock);
}