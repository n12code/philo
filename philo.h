/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:38:15 by nbodin            #+#    #+#             */
/*   Updated: 2025/05/27 09:12:23 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stddef.h>
# include <sys/types.h>

struct s_data;

typedef struct s_philo {
	int			id;
	int			meal_count;
	int			eating;
	t_data		*data;
	pthread_t	th;
	u_int64_t	time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}				t_philo;

typedef struct s_data {
	int			meals_num;
	int			dead;
	int			finished;
	int			philos_num;
	t_philo		*philos;
	u_int64_t	sleep_time;
	u_int64_t	death_time;
	u_int64_t	eat_time;
	u_int64_t	start_time;
	pthread_t	*th_arr;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}				t_data;

#endif