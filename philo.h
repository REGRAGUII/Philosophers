/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:41:28 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/16 18:39:48 by yregragu         ###   ########.fr       */
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
	int	start;
	int	end;
	pthread_mutex_t end_mutex;
}   data_t;

typedef struct	philo_s
{
	int	id;
	int eating_times;
	int	last_meal;
	pthread_t	thread;
	struct philo_s	*left;
	struct philo_s	*right;
	pthread_mutex_t	fork;
	data_t	*data;
}	philo_t;



int	check_args(char **av, int ac);
int	ft_isdigit(int x);
int	ft_atoi(char *str);
int	is_int(char *args);


#endif