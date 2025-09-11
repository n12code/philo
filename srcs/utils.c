/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:06 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/11 09:58:37 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_us(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000000LL) + tv.tv_usec);
}

long long	get_time(void)
{
	return (get_time_us() / 1000LL);
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
		destroy_mutex_safely(&data->forks[i].mutex);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		destroy_mutex_safely(&data->philos[i].philo_mutex);
		i++;
	}
	destroy_mutex_safely(&data->last_meal_lock);
	destroy_mutex_safely(&data->meals_eaten_lock);
	destroy_mutex_safely(&data->stop_lock);
	destroy_mutex_safely(&data->print_lock);
	destroy_mutex_safely(&data->eating_lock);
	destroy_mutex_safely(&data->log_mutex);
	destroy_mutex_safely(&data->death_mutex);
	free_log_lst(data->log_lst);
}
