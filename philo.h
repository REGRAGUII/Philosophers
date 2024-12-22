/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:41:28 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/22 19:04:00 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

#define ERROR_ARGS "The correct input: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]"  


typedef struct	data_s
{
	int	N_philo;
	int	T_die;
	int	T_eat;
	int	T_sleep;
	int	NofT_Eat;
	int	end;
	long	start_time;
	pthread_mutex_t end_mutex;
	pthread_mutex_t print;
	pthread_t	checker;
	pthread_mutex_t time_mutex;
	pthread_mutex_t finished_mutex;
	int				finished;
}   data_t;

typedef struct	philo_s
{
	int	id;
	long	last_meal;
	int		n_eats;
	pthread_t	thread;
	struct philo_s	*left;
	struct philo_s	*right;
	pthread_mutex_t	fork;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t eating_times;
	data_t	*data;
}	philo_t;


void	my_printf(char *msg, philo_t *philo);
int		check_args(char **av, int ac);
int		ft_isdigit(int x);
int		ft_atoi(char *str);
int		is_int(char *args);
void	clean(data_t *data, philo_t *philo);
void	ft_lstadd_back(philo_t *lst, philo_t *new);
long	get_time(void);
int		opt_sleep(size_t milliseconds, data_t *data);
void	*checker(void *param);


#endif