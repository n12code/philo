/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:00:51 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/10 23:59:16 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_owner(t_philo *philos, t_mutex *fork)
{
	int	owner;

	owner = 0;
	lock_safely(&philos->philo_mutex);
	if (philos->id == fork->owner)
		owner = 1;
	unlock_safely(&philos->philo_mutex);
	return (owner);
}

void	assign_forks(t_philo *philos,
	t_mutex **first_fork, t_mutex **second_fork)
{
	*first_fork = philos->left_fork;
	*second_fork = philos->right_fork;
	if (philos->data->n_philos % 2 == 0 && philos->id % 2 == 0)
	{
		*first_fork = philos->right_fork;
		*second_fork = philos->left_fork;
	}
}

void	try_to_release_forks(t_philo *philos,
	t_mutex **first_fork, t_mutex **second_fork)
{
	if (check_owner(philos, *first_fork))
		release_fork(*first_fork);
	if (check_owner(philos, *second_fork))
		release_fork(*second_fork);
}

int	take_forks_loop(t_philo *philos,
	t_mutex **first_fork, t_mutex **second_fork)
{
	if (!check_owner(philos, *first_fork)
		&& try_take_fork(*first_fork, philos->id))
		log_action(philos->data, philos->id, FORK, YELLOW);
	if (check_owner(philos, *first_fork)
		&& !check_owner(philos, *second_fork)
		&& try_take_fork(*second_fork, philos->id))
		log_action(philos->data, philos->id, FORK, YELLOW);
	if (check_owner(philos, *first_fork) && check_owner(philos, *second_fork))
		return (0);
	if (get_philos_state(philos->data))
	{
		if (check_owner(philos, *first_fork))
			release_fork(*first_fork);
		if (check_owner(philos, *second_fork))
			release_fork(*second_fork);
		return (1);
	}
	return (2);
}

void	drop_forks(t_philo *philos)
{
	release_fork(philos->right_fork);
	release_fork(philos->left_fork);
}
