#include "philo.h"

void clean_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->data->forks[philo->philo_id]);
    pthread_mutex_unlock(&philo->data->forks[(philo->philo_id + 1) % philo->data->number_of_philosophers]);
}

void take_forks(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->forks[philo->philo_id]);
    printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->philo_id);
    pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->number_of_philosophers]);

    printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->philo_id);
}

void philo_sleep(t_philo *philo)
{
    printf("%ld %d is sleeping\n", get_time() - philo->data->start_time, philo->philo_id);
    usleep(philo->data->time_to_sleep * 1000);
}

void eat(t_philo *philo)
{
    printf("%ld %d is eating\n", get_time() - philo->data->start_time , philo->philo_id);
    philo->last_meal_time = get_time();
    philo->times_eaten++;
    usleep(philo->data->time_to_eat * 1000);
}