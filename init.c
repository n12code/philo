/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:24:47 by nbodin            #+#    #+#             */
/*   Updated: 2025/05/27 09:51:39 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->philos_num = (int) argv[1];
	data->death_time = (u_int64_t) argv[2];
	data->eat_time = (u_int64_t) argv[3];
	data->sleep_time = (u_int64_t)argv[4];
	if (argv[5] && argc == 6)
		data->meals_num = argv[5];
	else
		data->meals_num = -1;
	if (data->philos <= 0 || data->philos > 200 || )
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->write, NULL);
	return (0);
}

int	init(t_data	*data, int argc, char **argv)
{
	//init	data
	//init philo
	//init forks
}