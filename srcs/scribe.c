/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scribe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:15:59 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 11:16:01 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_2.h"

int print_logs(t_data *data, long long delay)
{
    t_log *current;
    t_log *prev;
    int died;
    long long now;

    lock_safely(&data->log_mutex);
    now = (get_time_us() - data->start_time) / 1000LL;
    current = data->log_lst;
    if (current == NULL)
    {
        unlock_safely(&data->log_mutex);
        return (0);
    }
    died = 0;
    while (current != NULL && !died)
    {
        if (current->timestamp > now - delay)
            break;
        display_log(current);
        prev = current;
        died = (strncmp(current->action, "died", 4) == 0);
        current = current->next;
        free(prev);
    }
    data->log_lst = current;
    unlock_safely(&data->log_mutex);
    return (died);
}

void *scribe_routine(void *data_ptr)
{
    t_data *data;
    int died;

    data = (t_data *)data_ptr;
    if (!data)
        return (NULL);
    died = 0;
    ft_usleep_precise(SCRIBE_TIME_GAP * 1000LL, data);
    while (!died && !dinner_is_over(data))
    {
        died = print_logs(data, SCRIBE_TIME_GAP);
        ft_usleep_precise(SCRIBE_SLEEP_TIME * 1000LL, data);
    }
    if (!died)
        print_logs(data, 0);
    return (NULL);
}