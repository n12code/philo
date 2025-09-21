/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:06 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/21 17:05:48 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>

long long	get_time_us(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000000LL) + tv.tv_usec);
}

void	sleep_till(long long target_time)
{
	long long	diff;

	diff = target_time * 1000LL - get_time_us();
	while (diff > 0)
	{
		usleep(diff / 2);
		diff = target_time * 1000LL - get_time_us();
	}
}

void	ft_usleep_precise(long long sleep_time_us, t_data *data)
{
	long long	start;
	long long	target;

	start = get_time_us();
	target = start + sleep_time_us;
	usleep(sleep_time_us - 100);
	while (!get_philos_state(data) && get_time_us() < target)
		;
}

void	free_data(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->n_philos)
	{
		destroy_mutex_safely(&data->forks[i]);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		destroy_mutex_safely(&data->philos[i].philo_mutex);
		i++;
	}
	destroy_mutex_safely(&data->stop_lock);
	destroy_mutex_safely(&data->log_mutex);
	free_log_lst(data->log_lst);
}
