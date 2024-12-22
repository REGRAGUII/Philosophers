/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_sleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:35:45 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/22 16:17:02 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	opt_sleep(size_t milliseconds, data_t *data)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
	{
		pthread_mutex_lock(&data->end_mutex);
		if (!data->end)
		{
			pthread_mutex_unlock(&data->end_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->end_mutex);
		usleep(150);
	}
	return (0);
}

int	is_dead(philo_t *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	if (get_time() - philo->last_meal > philo->data->T_die)
	{
		pthread_mutex_unlock(&philo->last_meal_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
		philo->data->end = 0;
		pthread_mutex_unlock(&philo->data->end_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (0);
}

int	check_time(philo_t *philo)
{
	long time;

	
	if (is_dead(philo))
	{
		pthread_mutex_lock(&philo->data->print);
		time = get_time() - philo->data->start_time;
		printf("%ld  %d  died\n",time,  philo->id);
		pthread_mutex_unlock(&philo->data->print);
		return (1);
	}
	return (0);
}

int	check_eats(philo_t *philo)
{
	pthread_mutex_lock(&philo->data->eats_mutex);
	if (philo->data->eats == philo->data->N_philo - 1)
	{
		pthread_mutex_unlock(&philo->data->eats_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->eats_mutex);
	return (0);
} 
void	*checker(void *param)
{
	philo_t	*philo;

	philo = (philo_t *)param;
	while (1)
	{
		if (check_eats(philo))
		{
			return (NULL);
		}
		if (check_time(philo))
		{
			return (NULL);
		}
		if(philo->right)
			philo = philo->right;
	}
}