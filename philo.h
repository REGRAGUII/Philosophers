#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define ERROR_ARGS "The correct input: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]"  

typedef struct	philo_s
{
	int	N_philo;
	int	T_die;
	int	T_eat;
	int	T_sleep;
	int	NofT_Eat;
}   philo_t;

int	check_args(char **av, int ac, philo_t *philo);
int	ft_isdigit(int x);
int	ft_atoi(char *str);
int	is_int(char *args);


#endif