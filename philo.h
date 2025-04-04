#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>


typedef struct s_philo{
	int	philo_id;
	int times_eaten;
	int finished;
	long last_meal_time;
	long will_die_time;
	pthread_t thread;
	struct s_data *data;
} t_philo;

typedef struct s_data{
	long start_time;
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int times_must_eat;
	int philo_number;
	pthread_mutex_t mutex;
	pthread_mutex_t some_one_died_mutex;
	pthread_mutex_t *forks;
	t_philo *philo;
} t_data;

int check_and_insert(int argc , char **argv,t_data *data);
int	ft_atoi(const char *str);
void *philosopher(void *arg);

void clean_forks(t_philo *philo);
void take_forks(t_philo *philo);
void philo_sleep(t_philo *philo);
void eat(t_philo *philo);
int all_eaten(t_data *data);

long get_time(void);


#endif
