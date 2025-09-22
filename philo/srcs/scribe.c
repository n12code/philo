/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scribe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:15:59 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/11 17:59:17 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	update_current_prev(t_log **current, t_log **prev)
{
	*prev = *current;
	*current = (*current)->next;
	free(*prev);
}

int	print_logs(t_data *data, long long delay)
{
	t_log		*current;
	t_log		*prev;
	int			died;
	long long	now;

	pthread_mutex_lock(&data->log_mutex.mutex);
	now = (get_time_us() - data->start_time) / 1000LL;
	current = data->log_lst;
	prev = NULL;
	if (current == NULL)
	{
		pthread_mutex_unlock(&data->log_mutex.mutex);
		return (0);
	}
	died = 0;
	while (current != NULL && !died && current->timestamp <= now - delay)
	{
		display_log(current);
		died = (ft_strncmp(current->action, "died", 4) == 0);
		update_current_prev(&current, &prev);
	}
	data->log_lst = current;
	pthread_mutex_unlock(&data->log_mutex.mutex);
	return (died);
}

void	*scribe_routine(void *data_ptr)
{
	t_data	*data;
	int		died;

	data = (t_data *)data_ptr;
	if (!data)
		return (NULL);
	died = 0;
	ft_usleep_precise(SCRIBE_TIME_GAP * 1000LL, data);
	while (!died && !dinner_is_over(data) && !get_philos_state(data))
	{
		died = print_logs(data, SCRIBE_TIME_GAP);
		ft_usleep_precise(SCRIBE_SLEEP_TIME * 1000LL, data);
	}
	if (!died)
		print_logs(data, 0);
	return (NULL);
}
