#include "philo.h"

static int check_values(int argc,char **argv,t_data *data)
{
	int i = 1;
	int j = 0;
    while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return 1;
			j++;
		}
		i++;	
	}
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return 1;
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		return 1;
	return 0;
}

static void take_values(int argc,char **argv,t_data *data)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->times_must_eat = -1;
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	data->philo_number = 0;
}

int check_and_insert(int argc , char **argv,t_data *data)
{
    int i  =0;
	if (argc < 5 || argc > 6)
		return 1;
	if (check_values(argc,argv,data))
		return 1;
	take_values(argc,argv,data);
	return 0;

}