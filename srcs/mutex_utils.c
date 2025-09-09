/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:16:45 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/09 18:26:43 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_safely(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) != 0)
		return (1);
	return (0);
}

int	unlock_safely(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) != 0)
		return (1);
	return (0);
}

int	try_take_fork(t_mutex *fork)
{
	int	success;

	success = 0;
	lock_safely(&fork->mutex);
	if (fork->status == 0)
	{
		fork->status = 1;
		success = 1;
	}
	unlock_safely(&fork->mutex);
	return (success);
}

void	release_fork(t_mutex *fork)
{
	lock_safely(&fork->mutex);
	fork->status = 0;
	unlock_safely(&fork->mutex);
}
