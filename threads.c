/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:36:12 by nbodin            #+#    #+#             */
/*   Updated: 2025/05/28 19:45:13 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*supervisor(void *philo_ptr)
{
	//check if the philo isnt dead, if so is he eating
	//check that he ate all his meals, if so make him finished
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *) philo_ptr;
	philo->time_to_die = philo->data->death_time + get_time();
	if (pthread_create(&philo->th, NULL, &supervisor, (void *) philo))
		return ((void *) 1);
	while (1)
	{
		pthread_mutex_lock(&philo->data->lock);
		if (philo->data->dead == 1)
		{
			pthread_mutex_unlock(&philo->data->lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->lock);
		eat(philo);
		print_message();//thinking
		ft_usleep(100);
	}
	if (pthread_join(philo->th, NULL))
		return ((void *) 1);
	return ((void *) 0);
}

void	*monitor(void *data_ptr)
{
	t_data	*data;
	
	data = (t_data *) data_ptr;
	while (1)
	{
		pthread_mutex_lock(&data->lock);
		if (data->dead == 1)
		{
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		if (data->finished >= data->philos_num)
		{
			data->dead = 1;
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		pthread_mutex_unlock(&data->lock);
		ft_usleep(100);
	}
	return ((void *) 0);
}

int	launch_threads(t_data *data)
{
	pthread_t	th;
	size_t		i;

	i = 0;
	data->start_time = get_time();
	if (data->meals_num > 0)
	{
		if (pthread_create(&th, NULL, &monitor, (void *) &data))
			return (1);
	}
	while (i < data->philos_num)
	{
		if (pthread_create(&data->th_arr[i], NULL, &routine, (void *) &data->philos[i]))
			return (1);
		i++;
		ft_usleep(1);
	}
	i = 0;
	while (i < data->philos_num)
	{
		if (pthread_join(data->th_arr[i], NULL))
			return (1);
		i++;
	}
	return (0);
}