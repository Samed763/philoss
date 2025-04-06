#include "philo.h"
void join_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_philosophers)
    {
        // Eğer thread detach edilmemişse, pthread_join ile beklenebilir
        if (pthread_join(data->philo[i].thread, NULL) == 0)
        {
            printf("Thread %d joined successfully.\n", i);
        }
        else
        {
            printf("Thread %d is detached or already finished.\n", i);
        }
        i++;
    }
}
void clear_malloc(t_data *data)
{
    int i;

    i = 0;
    if (&data->some_one_died_mutex)
        pthread_mutex_destroy(&data->some_one_died_mutex);
    while (i < data->number_of_philosophers)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    if (data->philo)
        free(data->philo);
    if (data->forks)
        free(data->forks);
    if (data)
        free(data);
}

void *philo_check(void *arg)
{
    t_data *data = (t_data *)arg;
    pthread_mutex_lock(&data->some_one_died_mutex);
    
    while (1)
    {int i = 0;
        while (i < data->number_of_philosophers)
        {
            if (get_time() - data->philo[i].last_meal_time > data->time_to_die)
            {
                printf("%ld %d died-------------------------------|sss>\n", get_time() - data->start_time, data->philo[i].philo_id);
                data->end = 1; 
                pthread_mutex_unlock(&data->some_one_died_mutex);
                pthread_exit(NULL);
            }
            if (all_eaten(data)) // Eğer tüm filozoflar yemeğini yediyse
            {
                data->end = 1;
                pthread_mutex_unlock(&data->some_one_died_mutex);
                pthread_exit(NULL); // Kontrol thread'ini sonlandır
            }
            i++;
        }
        usleep(100);
    }
    pthread_exit(NULL);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
    {
        if (philo->data->end)
            break;
        if (philo->data->times_must_eat != -1 && philo->times_eaten >= philo->data->times_must_eat)
        {
            philo->finished = 1;
            break;
        }
        take_forks(philo);
        eat(philo);
        clean_forks(philo);
        philo_sleep(philo);
        printf("%ld %d is thinking\n", get_time() - philo->data->start_time, philo->philo_id);
    }
    pthread_exit(NULL);
}

int start_threads(t_data *data)
{
    int i;

    i = 0;
    pthread_t check_thread;
    if (pthread_create(&check_thread, NULL, philo_check, data))
    {
        printf("Error\n");
        usleep(1000);
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

    return (0);
}

void to_do_list(int argc, char *argv[], t_data *data)
{
    int i = 0;
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
    while (i < data->number_of_philosophers)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        data->philo[i].philo_id = i;
        data->philo[i].data = data;
        data->philo[i].times_eaten = 0;
        data->philo[i].last_meal_time = get_time();
        data->philo[i].finished = 0;
        i++;
    }
    data->end = 0;
    pthread_mutex_init(&data->some_one_died_mutex, NULL);
    data->start_time = get_time();
    if ( ft_atoi( argv[1]) ==1) 
    {
		usleep(ft_atoi(argv[2]) * 1000);
		printf("%ld %d died-------------------------------|sss>\n", get_time() - data->start_time, data->philo[i].philo_id);  
        exit(0); 
	}
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
    //sleep(2);
    pthread_mutex_unlock(&data->some_one_died_mutex);
    join_threads(data);//-------------------------------//
    clear_malloc(data);

    return 0;
}
