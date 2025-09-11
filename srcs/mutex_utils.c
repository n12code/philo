/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:16:45 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/11 16:08:13 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	try_take_fork(t_mutex *fork, int philo_id)
{
	int	success;

	success = 0;
	pthread_mutex_lock(&fork->mutex);
	if (fork->owner == -1)
	{
		fork->owner = philo_id;
		success = 1;
	}
	pthread_mutex_unlock(&fork->mutex);
	return (success);
}

void	release_fork(t_mutex *fork)
{
	pthread_mutex_lock(&fork->mutex);
	fork->owner = -1;
	pthread_mutex_unlock(&fork->mutex);
}
