#ifndef PHILO_2_H
# define PHILO_2_H

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
	pthread_t 		completion_monitor;
    pthread_t       scribe;
    int             nbr_monitors;
    t_monitoring_data *monitor_data;

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
void	*completion_monitor_routine(void *data_ptr);

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