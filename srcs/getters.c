/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:44:03 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/01 17:20:50 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_philos_state(t_data *data)
{
	int	philos_state;
	
	pthread_mutex_lock(&data->state_lock);
	philos_state = data->stop;
	pthread_mutex_unlock(&data->state_lock);
	return (philos_state);	
}