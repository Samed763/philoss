#include "philo.h"

void *philo_check(void *arg)
{
    t_data *data = (t_data *)arg;
    pthread_mutex_lock(&data->some_one_died_mutex);

    while (1)
    {
        for (int i = 0; i < data->number_of_philosophers; i++)
        {
            if (get_time() - data->philo[i].last_meal_time > data->time_to_die)
            {
                printf("%ld %d died-------------------------------|sss>\n", get_time(), data->philo[i].philo_id);
                pthread_mutex_unlock(&data->some_one_died_mutex);
                return NULL;
            }
            if (all_eaten(data))
            {
                pthread_mutex_unlock(&data->some_one_died_mutex);
                return NULL;
            }
        }
        usleep(100);
    }
    return NULL;
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
    {
        if (philo->data->times_must_eat != -1 && philo->times_eaten >= philo->data->times_must_eat)
        {
            philo->finished = 1;
            break;
        }
        take_forks(philo);
        eat(philo);
        clean_forks(philo);
        philo_sleep(philo);
        printf("%ld %d is thinking\n", get_time(), philo->philo_id);
    }
    return NULL;
}

int start_threads(t_data *data)
{
    int i;
    i = 0;
    pthread_t check_thread;
    if (pthread_create(&check_thread, NULL, philo_check, data))
    {
        printf("Error\n");
        return (1);
    }
    pthread_detach(check_thread);
    while (i < data->number_of_philosophers)
    {
        if (pthread_create(&data->philo[i].thread, NULL, philo_routine, &data->philo[i]))
        {
            printf("Error\n");
            return (1);
        }
        usleep(100);
        pthread_detach(data->philo[i].thread);
        i++;
    }
    
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        if (pthread_create(&data->philo[i].thread, NULL, philo_routine, &data->philo[i]))
        {
            printf("Error\n");
            return (1);
        }
        usleep(100);
        pthread_detach(data->philo[i].thread);
    }

    return (0);
}

void to_do_list(int argc, char *argv[], t_data *data)
{
    if (check_and_insert(argc, argv, data))
    {
        printf("Error\n");
        return;
    }
    data->philo = malloc(sizeof(t_philo) * data->number_of_philosophers);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
    if (!data->philo || !data->forks)
    {
        printf("Error\n");
        return;
    }
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        data->philo[i].philo_id = i;
        data->philo[i].data = data;
        data->philo[i].times_eaten = 0;
        data->philo[i].last_meal_time = get_time();
    }
    pthread_mutex_init(&data->some_one_died_mutex, NULL);
    pthread_mutex_init(&data->mutex, NULL);
}

int main(int argc, char *argv[])
{
    t_data *data;

    data = malloc(sizeof(t_data)); // Bellek tahsisi
    if (!data)
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    to_do_list(argc, argv, data);
    start_threads(data);

    pthread_mutex_lock(&data->some_one_died_mutex);
    pthread_mutex_unlock(&data->some_one_died_mutex);

    return 0;
}
