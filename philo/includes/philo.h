/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:22:55 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/21 17:06:56 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <pthread.h>

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"

# define FORK "has taken a fork"
# define EATING "is eating"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define DIED "died"

# define CHUNK_SIZE 20
# define SCRIBE_SLEEP_TIME 59
# define SCRIBE_TIME_GAP 67
# define MONITOR_SLEEP 100
# define FORK_RETRY_DELAY 50

struct	s_data;

typedef struct s_mutex
{
	pthread_mutex_t		mutex;
	int					owner;
	int					init;
}						t_mutex;

typedef struct s_log
{
	long long			timestamp;
	int					philo_id;
	char				action[50];
	char				color[16];
	struct s_log		*next;
}						t_log;

typedef struct s_philo
{
	int					id;
	int					eating;
	int					meals_eaten;
	long long			last_meal;
	long long			meal_end_time;
	long long			wake_up_time;
	pthread_t			thread;
	t_mutex				*left_fork;
	t_mutex				*right_fork;
	t_mutex				philo_mutex;
	struct s_data		*data;
}						t_philo;

typedef struct s_monitoring_data
{
	int					id;
	int					nbr_monitors;
	struct s_data		*data;
}						t_monitoring_data;

typedef struct s_data
{
	size_t				n_philos;
	int					n_meals;
	int					stop;
	int					i_scribe;
	int					i_philos;
	int					i_monitors;
	int					i_comp_monitor;
	int					nbr_monitors;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	long long			start_time;
	t_mutex				forks[200];
	t_mutex				stop_lock;
	t_mutex				log_mutex;
	pthread_t			monitors[10];
	pthread_t			completion_monitor;
	pthread_t			scribe;
	t_monitoring_data	monitor_data[10];
	t_philo				philos[200];
	t_log				*log_lst;
}						t_data;

int			philos_eat(t_philo *philos);
int			take_forks(t_philo *philos);
int			philos_sleep(t_philo *philos);
int			philos_think(t_philo *philos);

int			ft_atoi(char *str);
int			check_input_digits(char **argv);
int			check_input_valid(int argc, char **argv);
long		ft_atol(char *str, int *overflow);
void		overflow_checker(char c, long *nb, int *overflow);

int			check_owner(t_philo *philos, t_mutex *fork);
int			take_forks_loop(t_philo *philos,
				t_mutex **first_fork, t_mutex **second_fork);
void		drop_forks(t_philo *philos);
void		assign_forks(t_philo *philos,
				t_mutex **first_fork, t_mutex **second_fork);
void		try_to_release_forks(t_philo *philos,
				t_mutex **first_fork, t_mutex **second_fork);

int			get_philos_state(t_data *data);
int			get_meals_eaten(t_philo *philo);
int			get_eating_state(t_philo *philo);
long long	get_time(void);
long long	get_last_meal_time(t_philo *philo);

int			init_forks(t_data *data);
int			init_philos(t_data *data);
int			init_monitor_data(t_data *data);
int			init_mutex_safely(t_mutex *mutex);
int			init_data(int argc, char **argv, t_data *data);

int			log_action(t_data *data, int philo_id, const char *action,
				const char *color);
void		free_log_lst(t_log *log_lst);
void		display_log(const t_log *log);
t_log		*add_log(t_log *log_lst, t_log *log);
t_log		*create_log(long long timestamp, int philo_id, const char *action,
				const char *color);

int			threads(t_data *data);
int			main(int argc, char **argv);
int			check_input(int argc, char **argv);
int			philosophers(int argc, char **argv);
int			init(int argc, char **argv, t_data *data);

int			philo_died(t_philo *philo);
int			check_philosopher_status(t_data *data, int philosopher_id,
				int *death_detected);
void		*monitor_routine(void *monitor_data_ptr);
void		*completion_monitor_routine(void *data_ptr);

int			print_logs(t_data *data, long long delay);
void		*scribe_routine(void *data_ptr);
void		update_current_prev(t_log **current, t_log **prev);

int			join_threads(t_data *data);
int			create_threads(t_data *data);
int			launch_monitors(t_data *data);
void		*routine(void *philos_pointer);

int			dinner_is_over(t_data *data);
int			handle_single_philo(t_philo *philos);
int			get_nbr_chunks(int num_philosophers);
void		change_philos_state(t_data *data);
void		destroy_mutex_safely(t_mutex *mutex);

void		free_data(t_data *data);
void		sleep_till(long long target_time);
void		ft_usleep_precise(long long sleep_time_us, t_data *data);
long long	get_time_us(void);

int			try_take_fork(t_mutex *fork, int philo_id);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		release_fork(t_mutex *fork);
void		*ft_memset(void *s, int c, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, int size);

#endif