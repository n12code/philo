/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:16:02 by nbodin            #+#    #+#             */
/*   Updated: 2025/06/03 15:57:14 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(char *message, t_philo *philo)
{
	u_int64_t	time;

	pthread_mutex_lock(&philo->data->lock);
	time = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->lock);
	pthread_mutex_lock(&philo->data->write);
	if (ft_strncmp(DIED, message) == 0 && philo->data->dead == 0)
	{
		printf("%llu %d %s\n", time, philo->id, message);
		philo->data->dead = 1;
	}
	if (philo->data->dead == 0)
		printf("%llu %d %s\n", time, philo->id, message);
	pthread_mutex_unlock(&philo->data->write);
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->data->lock);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->l_fork);
	print_message(FORK, philo);
	pthread_mutex_lock(&philo->r_fork);
	print_message(FORK, philo);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->l_fork);
	pthread_mutex_unlock(&philo->l_fork);
	print_message(SLEEPING, philo);
	pthread_mutex_lock(&philo->data->lock);
	ft_usleep(philo->data->sleep_time);
	pthread_mutex_unlock(&philo->data->lock);
}

void	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	pthread_mutex_lock(&philo->data->lock);
	philo->time_to_die = get_time() + philo->data->death_time;
	print_message(EATING, philo);
	philo->meal_count++;
	ft_usleep(philo->data->eat_time);
	pthread_mutex_unlock(&philo->data->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
}