/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:38:15 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/02 10:01:57 by nbodin           ###   ########lyon.fr   */
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

# define FORK "has taken a fork"
# define EATING "is eating"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define DIED "died"

struct s_data;

typedef struct s_philo {
    int             id;             // Philosopher ID (1..n)
    int             meals_eaten;    // How many times this philo ate
	int				eating;			// State of this philo
    u_int64_t		last_meal;      // Timestamp (ms) of last meal

    pthread_t       thread;         // Thread handle

    pthread_mutex_t *left_fork;     // Pointer to left fork
    pthread_mutex_t *right_fork;    // Pointer to right fork

    struct s_data   *data;          // Back reference to global data
}   t_philo;


typedef struct s_data {
    size_t          n_philos;       // Number of philosophers
    u_int64_t		time_to_die;    // Max time without eating (ms)
    u_int64_t		time_to_eat;    // Eating duration (ms)
    u_int64_t		time_to_sleep;  // Sleeping duration (ms)
    int             n_meals;        // Optional: number of meals required (-1 if none)

    u_int64_t		start_time;     // Simulation start timestamp (ms)
    int             stop;           // Flag to stop simulation (set by monitor)

    pthread_mutex_t *forks;         // Array of fork mutexes
    pthread_mutex_t print_lock;     // Mutex to protect console output
    pthread_mutex_t state_lock;     // Protects shared flags (stop, meal counters, etc.)

	pthread_t		monitor;		// Thread that checks if philosophers are alive or full

    t_philo         *philos;        // Array of philosopher structs
}   t_data;


// main.c
int	threads(t_data *data);
int	main(int argc, char **argv);
int	check_input(int argc, char **argv);
int	philosophers(int argc, char **argv);
int	init(int argc, char **argv, t_data *data);

// check_input.c
int	ft_atoi(char *str);
int	check_input_digits(char **argv);
int	check_input_valid(int argc, char **argv);

// init.c
int	init_forks(t_data *data);
int	init_philos(t_data *data);
int init_data(int argc, char **argv, t_data *data);

// threads.c
int	join_threads(t_data *data);
int	create_threads(t_data *data);

// utils.c
u_int64_t	get_time(void);
void		free_data(t_data *data);
void		print_message(t_philo *philos, char *msg);
void		ft_usleep(uint64_t sleep_time, t_data *data);


// getters.c
int	get_philos_state(t_data *data);

// actions.c
int		philos_eat(t_philo *philos);
int		take_forks(t_philo *philos);
void	drop_forks(t_philo *philos);
int		philos_sleep(t_philo *philos);
int		philos_think(t_philo *philos);



#endif