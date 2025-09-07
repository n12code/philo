/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:38:15 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/07 10:36:42 by nbodin           ###   ########lyon.fr   */
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
# include <stdint.h>
# include <string.h>

/* Color definitions for logging */
# define RED		"\x1b[31m"
# define GREEN		"\x1b[32m"
# define YELLOW		"\x1b[33m"
# define BLUE		"\x1b[34m"
# define CYAN		"\x1b[36m"
# define RESET		"\x1b[0m"

/* Action strings */
# define FORK "has taken a fork"
# define EATING "is eating"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define DIED "died"

/* Constants */
# define CHUNK_SIZE 20
# define SCRIBE_SLEEP_TIME 59
# define SCRIBE_TIME_GAP 67
# define MONITOR_SLEEP 100

struct s_data;

/* Log structure for advanced logging system */
typedef struct s_log {
    long long           timestamp;
    int                 philo_id;
    char                action[50];
    char                color[16];
    struct s_log        *next;
} t_log;

typedef struct s_philo {
    int             id;
    int             meals_eaten;
    int             eating;
    long long       last_meal;
    long long       meal_end_time;
    long long       wake_up_time;

    pthread_t       thread;
    pthread_mutex_t philo_mutex;

    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;

    struct s_data   *data;
} t_philo;

/* Monitoring data for chunked monitoring */
typedef struct s_monitoring_data {
    int             id;
    int             nbr_monitors;
    struct s_data   *data;
} t_monitoring_data;

typedef struct s_data {
    size_t          n_philos;
    long long       time_to_die;
    long long       time_to_eat;
    long long       time_to_sleep;
    int             n_meals;

    long long       start_time;
    int             stop;

    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t last_meal_lock;
    pthread_mutex_t meals_eaten_lock;
    pthread_mutex_t stop_lock;
    pthread_mutex_t eating_lock;
    pthread_mutex_t log_mutex;
    pthread_mutex_t death_mutex;

    pthread_t       *monitors;
    pthread_t       scribe;
    int             nbr_monitors;

    t_philo         *philos;
    t_log           *log_lst;
    int             someone_died;
} t_data;

/* Function prototypes */

/* main.c */
int     threads(t_data *data);
int     main(int argc, char **argv);
int     check_input(int argc, char **argv);
int     philosophers(int argc, char **argv);
int     init(int argc, char **argv, t_data *data);

/* check_input.c */
int     ft_atoi(char *str);
int     check_input_digits(char **argv);
int     check_input_valid(int argc, char **argv);

/* init.c */
int     init_forks(t_data *data);
int     init_malloc(t_data *data);
int     init_philos(t_data *data);
int     init_data(int argc, char **argv, t_data *data);

/* threads.c */
int     join_threads(t_data *data);
int     create_threads(t_data *data);
void    *routine(void *philos_pointer);

/* utils.c */
long long   get_time_us(void);
long long   get_time(void);
void        free_data(t_data *data);
int         handle_single_philo(t_philo *philos);
void        ft_usleep_precise(long long sleep_time, t_data *data);
void        sleep_till(long long target_time);

/* getters.c */
int         get_philos_state(t_data *data);
int         get_meals_eaten(t_philo *philo);
int         get_eating_state(t_philo *philo);
long long   get_last_meal_time(t_philo *philo);
int         get_someone_died(t_data *data);

/* actions.c */
int     philos_eat(t_philo *philos);
int     take_forks(t_philo *philos);
void    drop_forks(t_philo *philos);
int     philos_sleep(t_philo *philos);
int     philos_think(t_philo *philos);

/* monitor.c */
int     philo_full(t_philo *philo);
int     philo_died(t_philo *philo);
void    *monitor_routine(void *monitor_data_ptr);
void    change_philos_state(t_data *data);
int     get_nbr_chunks(int num_philosophers);

/* logging.c */
t_log   *create_log(long long timestamp, int philo_id, const char *action, const char *color);
t_log   *add_log(t_log *log_lst, t_log *log);
void    log_action(t_data *data, int philo_id, const char *action, const char *color);
int     display_log(const t_log *log);
void    free_log_lst(t_log *log_lst);

/* scribe.c */
void    *scribe_routine(void *data_ptr);
int     print_logs(t_data *data, long long delay);

/* mutex_utils.c */
int     lock_safely(pthread_mutex_t *mutex);
int     unlock_safely(pthread_mutex_t *mutex);

/* simulation_utils.c */
int     dinner_is_over(t_data *data);
int     is_simulation_over(t_data *data);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:00:00 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_log *create_log(long long timestamp, int philo_id, const char *action, const char *color)
{
    t_log *log;

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

t_log *add_log(t_log *log_lst, t_log *log)
{
    t_log *current;
    t_log *prev;

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

void log_action(t_data *data, int philo_id, const char *action, const char *color)
{
    t_log *log;
    long long timestamp;

    timestamp = (get_time_us() - data->start_time) / 1000LL;
    log = create_log(timestamp, philo_id, action, color);
    if (!log)
        return;
    
    lock_safely(&data->log_mutex);
    data->log_lst = add_log(data->log_lst, log);
    unlock_safely(&data->log_mutex);
}

int display_log(const t_log *log)
{
    return printf("%s%lld %d %s%s\n", log->color, log->timestamp, 
                  log->philo_id + 1, log->action, RESET);
}

void free_log_lst(t_log *log_lst)
{
    t_log *current;
    t_log *prev;

    if (log_lst == NULL)
        return;
    current = log_lst;
    while (current != NULL)
    {
        prev = current;
        current = current->next;
        free(prev);
    }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scribe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:00:00 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:34:09 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void change_philos_state(t_data *data)
{
    lock_safely(&data->stop_lock);
    data->stop = 1;
    unlock_safely(&data->stop_lock);
    
    lock_safely(&data->death_mutex);
    data->someone_died = 1;
    unlock_safely(&data->death_mutex);
}

int philo_died(t_philo *philo)
{
    long long time_since_last_meal;
    
    time_since_last_meal = get_time() - get_last_meal_time(philo);
    if (time_since_last_meal >= philo->data->time_to_die && !get_eating_state(philo))
        return (1);
    return (0);
}

int philo_full(t_philo *philo)
{
    if (get_meals_eaten(philo) >= philo->data->n_meals)
        return (1);
    return (0);
}

static int check_philosopher_status(t_data *data, int philosopher_id, int *death_detected)
{
    if (philo_died(&data->philos[philosopher_id]) && !(*death_detected))
    {
        log_action(data, philosopher_id, DIED, RED);
        change_philos_state(data);
        *death_detected = 1;
        return (0);
    }
    return (1);
}

void *monitor_routine(void *monitor_data_ptr)
{
    t_monitoring_data *monitor_data;
    t_data *data;
    int i;
    int death_detected;

    monitor_data = (t_monitoring_data *)monitor_data_ptr;
    data = monitor_data->data;
    death_detected = 0;
    
    while (!get_philos_state(data))
    {
        i = monitor_data->id;
        while (i < (int)data->n_philos)
        {
            if (check_philosopher_status(data, i, &death_detected) == 0)
                break;
            i += monitor_data->nbr_monitors;
        }
        if (dinner_is_over(data))
            break;
        usleep(MONITOR_SLEEP);
    }
    return (NULL);
}

int get_nbr_chunks(int num_philosophers)
{
    int chunks;

    chunks = num_philosophers / CHUNK_SIZE;
    if (num_philosophers % CHUNK_SIZE != 0)
        chunks++;
    return (chunks);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:00:00 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:00:00 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
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
    int someone_died;
    int dinner_over;

    someone_died = get_someone_died(data);
    if (someone_died)
        return (1);
    dinner_over = dinner_is_over(data);
    return (dinner_over);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_time_us(void)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL))
        return (0);
    return ((tv.tv_sec * 1000000LL) + tv.tv_usec);
}

long long get_time(void)
{
    return (get_time_us() / 1000LL);
}

void sleep_till(long long target_time)
{
    long long diff;

    diff = target_time * 1000LL - get_time_us();
    while (diff > 0)
    {
        usleep(diff / 2);
        diff = target_time * 1000LL - get_time_us();
    }
}

void ft_usleep_precise(long long sleep_time_us, t_data *data)
{
    long long start;
    long long target;

    start = get_time_us();
    target = start + sleep_time_us;
    usleep(sleep_time_us - 100);
    while (!get_philos_state(data) && get_time_us() < target)
        ;
}

int handle_single_philo(t_philo *philos)
{
    lock_safely(philos->left_fork);
    log_action(philos->data, philos->id, FORK, YELLOW);
    while (!get_philos_state(philos->data))
        usleep(1000);
    unlock_safely(philos->left_fork);
    change_philos_state(philos->data);
    return (1);
}

void free_data(t_data *data)
{
    size_t i;

    i = 0;
    if (data->forks)
    {
        while (i < data->n_philos)
        {
            pthread_mutex_destroy(&data->forks[i]);
            i++;
        }
        free(data->forks);
    }
    if (data->philos)
    {
        i = 0;
        while (i < data->n_philos)
        {
            pthread_mutex_destroy(&data->philos[i].philo_mutex);
            i++;
        }
        free(data->philos);
    }
    if (data->monitors)
        free(data->monitors);
    pthread_mutex_destroy(&data->last_meal_lock);
    pthread_mutex_destroy(&data->meals_eaten_lock);
    pthread_mutex_destroy(&data->stop_lock);
    pthread_mutex_destroy(&data->print_lock);
    pthread_mutex_destroy(&data->eating_lock);
    pthread_mutex_destroy(&data->log_mutex);
    pthread_mutex_destroy(&data->death_mutex);
    free_log_lst(data->log_lst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int get_philos_state(t_data *data)
{
    int philos_state;
    
    lock_safely(&data->stop_lock);
    philos_state = data->stop;
    unlock_safely(&data->stop_lock);
    return (philos_state);    
}

long long get_last_meal_time(t_philo *philo)
{
    long long last_meal;
    
    lock_safely(&philo->philo_mutex);
    last_meal = philo->last_meal;
    unlock_safely(&philo->philo_mutex);
    return (last_meal);    
}

int get_eating_state(t_philo *philo)
{
    int eating_state;
    
    lock_safely(&philo->philo_mutex);
    eating_state = philo->eating;
    unlock_safely(&philo->philo_mutex);
    return (eating_state);    
}

int get_meals_eaten(t_philo *philo)
{
    int meals_eaten;
    
    lock_safely(&philo->philo_mutex);
    meals_eaten = philo->meals_eaten;
    unlock_safely(&philo->philo_mutex);
    return (meals_eaten);
}

int get_someone_died(t_data *data)
{
    int someone_died;

    lock_safely(&data->death_mutex);
    someone_died = data->someone_died;
    unlock_safely(&data->death_mutex);
    return (someone_died);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philos_eat(t_philo *philos)
{
    if (take_forks(philos))
        return (1);
    
    lock_safely(&philos->philo_mutex);
    philos->last_meal = get_time();
    philos->eating = 1;
    philos->meal_end_time = get_time() + philos->data->time_to_eat;
    unlock_safely(&philos->philo_mutex);
    
    log_action(philos->data, philos->id, EATING, GREEN);
    
    lock_safely(&philos->philo_mutex);
    philos->meals_eaten++;
    unlock_safely(&philos->philo_mutex);
    
    sleep_till(philos->meal_end_time);
    drop_forks(philos);
    
    lock_safely(&philos->philo_mutex);
    philos->eating = 0;
    unlock_safely(&philos->philo_mutex);
    
    return (get_philos_state(philos->data));
}

int take_forks(t_philo *philos)
{
    if (get_philos_state(philos->data))
        return (1);
    if (philos->data->n_philos == 1)
        return (handle_single_philo(philos));
    
    if (philos->id % 2 == 0)
    {
        lock_safely(philos->left_fork);
        log_action(philos->data, philos->id, FORK, YELLOW);
        lock_safely(philos->right_fork);
        log_action(philos->data, philos->id, FORK, YELLOW);
    }
    else
    {
        lock_safely(philos->right_fork);
        log_action(philos->data, philos->id, FORK, YELLOW);
        lock_safely(philos->left_fork);
        log_action(philos->data, philos->id, FORK, YELLOW);
    }
    
    if (get_philos_state(philos->data))
    {
        unlock_safely(philos->right_fork);
        unlock_safely(philos->left_fork);
        return (1);
    }
    return (0);
}

void drop_forks(t_philo *philos)
{
    unlock_safely(philos->right_fork);
    unlock_safely(philos->left_fork);
}

int philos_think(t_philo *philos)
{
    if (get_philos_state(philos->data))
        return (1);
    log_action(philos->data, philos->id, THINKING, CYAN);
    if (philos->data->n_philos > 50)
        ft_usleep_precise(1000, philos->data);
    return (0);
}

int philos_sleep(t_philo *philos)
{
    long long wake_up_time;
    
    if (get_philos_state(philos->data))
        return (1);
    
    log_action(philos->data, philos->id, SLEEPING, BLUE);
    
    lock_safely(&philos->philo_mutex);
    wake_up_time = get_time() + philos->data->time_to_sleep;
    philos->wake_up_time = wake_up_time;
    unlock_safely(&philos->philo_mutex);
    
    sleep_till(wake_up_time);
    return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_forks(t_data *data)
{
    size_t i;

    i = 0;
    while (i < data->n_philos)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    i = 0;
    while (i < data->n_philos)
    {    
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
        i++;
    }    
    return (0);
}

int init_philos(t_data *data)
{
    size_t i;
    t_philo *philos;

    i = 0;
    philos = data->philos;
    data->start_time = get_time_us();
    while (i < data->n_philos)
    {
        philos[i].id = i;
        philos[i].meals_eaten = 0;
        philos[i].last_meal = data->start_time / 1000LL;
        philos[i].data = data;
        philos[i].eating = 0;
        philos[i].meal_end_time = 0;
        philos[i].wake_up_time = 0;
        pthread_mutex_init(&philos[i].philo_mutex, NULL);
        i++;
    }
    return (0);
}

int init_malloc(t_data *data)
{
    data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
    if (!data->forks)
        return (1);
    data->philos = malloc(data->n_philos * sizeof(t_philo));
    if (!data->philos)
        return (1);
    data->nbr_monitors = get_nbr_chunks(data->n_philos);
    data->monitors = malloc(data->nbr_monitors * sizeof(pthread_t));
    if (!data->monitors)
        return (1);
    return (0);
}

int init_data(int argc, char **argv, t_data *data)
{
    data->n_philos = (size_t) ft_atoi(argv[1]);
    data->time_to_die = (long long) ft_atoi(argv[2]);
    data->time_to_eat = (long long) ft_atoi(argv[3]);
    data->time_to_sleep = (long long) ft_atoi(argv[4]);
    data->n_meals = -1;
    if (argc == 6)
        data->n_meals = ft_atoi(argv[5]);
    data->stop = 0;
    data->someone_died = 0;
    data->log_lst = NULL;
    
    pthread_mutex_init(&data->print_lock, NULL);
    pthread_mutex_init(&data->last_meal_lock, NULL);
    pthread_mutex_init(&data->meals_eaten_lock, NULL);
    pthread_mutex_init(&data->stop_lock, NULL);
    pthread_mutex_init(&data->eating_lock, NULL);
    pthread_mutex_init(&data->log_mutex, NULL);
    pthread_mutex_init(&data->death_mutex, NULL);
    
    if (init_malloc(data))
        return (1);
    return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*   Updated: 2025/09/06 18:00:00 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *philos_pointer)
{
    t_philo *philos;

    philos = (t_philo *) philos_pointer;
    if (philos->data->n_philos > 50)
    {
        if (philos->id % 2 == 0)
            usleep(100);
    }
    while (!get_philos_state(philos->data))
    {
        if (philos_eat(philos))
            break;
        if (get_philos_state(philos->data))
            break;
        if (philos_sleep(philos))
            break;
        if (get_philos_state(philos->data))
            break;
        if (philos_think(philos))
            break;
    }
    return (NULL);
}

int create_threads(t_data *data)
{
    size_t i;
    t_philo *philos;
    t_monitoring_data *monitor_data;
    
    i = 0;
    philos = data->philos;
    
    // Allocate monitoring data
    monitor_data = malloc(data->nbr_monitors * sizeof(t_monitoring_data));
    if (!monitor_data)
        return (1);
    
    // Initialize monitoring data
    i = 0;
    while (i < (size_t)data->nbr_monitors)
    {
        monitor_data[i].id = i;
        monitor_data[i].nbr_monitors = data->nbr_monitors;
        monitor_data[i].data = data;
        i++;
    }
    
    // Create philosopher threads
    i = 0;
    while (i < data->n_philos)
    {
        if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]))
        {
            free(monitor_data);
            return (1);
        }
        i++;
    }
    
    // Create monitor threads
    i = 0;
    while (i < (size_t)data->nbr_monitors)
    {
        if (pthread_create(&data->monitors[i], NULL, &monitor_routine, &monitor_data[i]))
        {
            free(monitor_data);
            return (1);
        }
        i++;
    }
    
    // Create scribe thread
    if (pthread_create(&data->scribe, NULL, &scribe_routine, data))
    {
        free(monitor_data);
        return (1);
    }
    
    // Store monitor_data pointer for cleanup (you might want to add this to t_data structure)
    // For now we'll handle cleanup in join_threads
    
    return (0);
}

int join_threads(t_data *data)
{
    size_t i;
    
    i = 0;
    while (i < data->n_philos)
    {
        if (pthread_join(data->philos[i].thread, NULL))
            return (1);
        i++;
    }
    
    i = 0;
    while (i < (size_t)data->nbr_monitors)
    {
        if (pthread_join(data->monitors[i], NULL))
            return (1);
        i++;
    }
    
    if (pthread_join(data->scribe, NULL))
        return (1);
    
    return (0);
}