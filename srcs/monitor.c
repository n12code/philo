/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:16:15 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/10 22:57:04 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_died(t_philo *philo)
{
	long long	time_since_last_meal;

	time_since_last_meal = get_time_us() - get_last_meal_time(philo);
	if (time_since_last_meal >= philo->data->time_to_die * 1000LL
		&& !get_eating_state(philo))
		return (1);
	return (0);
}

int	philo_full(t_philo *philo)
{
	if (get_meals_eaten(philo) >= philo->data->n_meals)
		return (1);
	return (0);
}

static int	check_philosopher_status(t_data *data, int philosopher_id,
		int *death_detected)
{
	if (philo_died(&data->philos[philosopher_id]) && !(*death_detected))
	{
		log_action(data, philosopher_id, DIED, RED);
		change_philos_state(data);
		*death_detected = 1;
		return (0);
	}
	return (1);
}

void	*monitor_routine(void *monitor_data_ptr)
{
	t_monitoring_data	*monitor_data;
	t_data				*data;
	int					i;
	int					death_detected;

	monitor_data = (t_monitoring_data *)monitor_data_ptr;
	data = monitor_data->data;
	death_detected = 0;
	while (!get_philos_state(data))
	{
		i = monitor_data->id;
		while (i < (int)data->n_philos)
		{
			if (check_philosopher_status(data, i, &death_detected) == 0)
				return (NULL);
			i += monitor_data->nbr_monitors;
		}
		usleep(MONITOR_SLEEP);
	}
	return (NULL);
}

void	*completion_monitor_routine(void *data_ptr)
{
	t_data	*data;
	size_t	i;

	data = (t_data *)data_ptr;
	if (data->n_meals == -1)
		return (NULL);
	while (!get_philos_state(data))
	{
		i = 0;
		while (i < data->n_philos
			&& get_meals_eaten(&data->philos[i]) >= data->n_meals)
			i++;
		if (i == data->n_philos)
		{
			change_philos_state(data);
			break ;
		}
		usleep(MONITOR_SLEEP);
	}
	return (NULL);
}
