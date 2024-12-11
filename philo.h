/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:41:28 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/11 04:28:35 by yregragu         ###   ########.fr       */
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

#define ERROR_ARGS "The correct input: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]"  


typedef struct	data_s
{
	int	N_philo;
	int	T_die;
	int	T_eat;
	int	T_sleep;
	int	NofT_Eat;
	int	start;
}   data_t;

typedef struct	philo_s
{
	int	id;
	int eating_times;
	int	last_meal;
	pthread_t	*thread;
	struct philo_s	*left;
	struct philo_s	*right;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
	data_t	*data;
}	philo_t;



int	check_args(char **av, int ac);
int	ft_isdigit(int x);
int	ft_atoi(char *str);
int	is_int(char *args);


#endif