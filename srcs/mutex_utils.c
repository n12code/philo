/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:16:45 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 11:16:53 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_2.h"

int lock_safely(pthread_mutex_t *mutex)
{
    if (pthread_mutex_lock(mutex) != 0)
        return (1);
    return (0);
}

int unlock_safely(pthread_mutex_t *mutex)
{
    if (pthread_mutex_unlock(mutex) != 0)
        return (1);
    return (0);
}