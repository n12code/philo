/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:14:10 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/10 19:57:46 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_log	*create_log(long long timestamp, int philo_id, const char *action,
		const char *color)
{
	t_log	*log;

	log = malloc(sizeof(t_log));
	if (!log)
		return (NULL);
	memset(log, 0, sizeof(t_log));
	log->timestamp = timestamp;
	log->philo_id = philo_id;
	strcpy(log->action, action);
	strcpy(log->color, color);
	log->next = NULL;
	return (log);
}

t_log	*add_log(t_log *log_lst, t_log *log)
{
	t_log	*current;
	t_log	*prev;

	if (log_lst == NULL)
		return (log);
	current = log_lst;
	prev = NULL;
	while (current != NULL && log->timestamp >= current->timestamp)
	{
		prev = current;
		current = current->next;
	}
	if (prev == NULL)
	{
		log->next = log_lst;
		return (log);
	}
	else
	{
		prev->next = log;
		log->next = current;
		return (log_lst);
	}
}

int	log_action(t_data *data, int philo_id, const char *action,
		const char *color)
{
	t_log		*log;
	long long	timestamp;

	timestamp = (get_time_us() - data->start_time) / 1000LL;
	log = create_log(timestamp, philo_id, action, color);
	if (!log)
	{
		change_philos_state(data);
		return (1);
	}
	lock_safely(&data->log_mutex);
	data->log_lst = add_log(data->log_lst, log);
	unlock_safely(&data->log_mutex);
	return (0);
}

int	display_log(const t_log *log)
{
	return (printf("%s%lld %d %s%s\n", log->color, log->timestamp, log->philo_id
			+ 1, log->action, RESET));
}

void	free_log_lst(t_log *log_lst)
{
	t_log	*current;
	t_log	*prev;

	if (log_lst == NULL)
		return ;
	current = log_lst;
	while (current != NULL)
	{
		prev = current;
		current = current->next;
		free(prev);
	}
}
