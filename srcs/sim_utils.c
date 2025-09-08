/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:17:33 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/08 15:51:28 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int dinner_is_over(t_data *data)
{
    size_t i;
    int meals_needed;

    i = 0;
    meals_needed = data->n_meals;
    if (meals_needed == -1)
        return (0);
    while (i < data->n_philos)
    {
        if (get_meals_eaten(&data->philos[i]) < meals_needed)
            return (0);
        i++;
    }
    return (1);
}

int is_simulation_over(t_data *data)
{
    int stop;
    int dinner_over;

    stop = get_philos_state(data);
    if (stop)
        return (1);
    dinner_over = dinner_is_over(data);
    return (dinner_over);
}