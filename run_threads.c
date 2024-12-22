/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_threads.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 19:13:18 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/22 20:14:18 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(philo_t *philo)
{
	if (stats(philo))
		return (1);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		my_printf("has taken a fork", philo);
		pthread_mutex_lock(&philo->left->fork);
		my_printf("has taken a fork", philo);
	}
	else
	{
		pthread_mutex_lock(&philo->left->fork);
		my_printf("has taken a fork", philo);
		pthread_mutex_lock(&philo->fork);
		my_printf("has taken a fork", philo);
	}
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	my_printf("is eating", philo);
	if (opt_sleep(philo->data->t_eat, philo->data))
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);
		return (1);
	}
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->left->fork);
	return (0);
}

int	sleeping(philo_t *philo)
{
	if (stats(philo))
		return (1);
	my_printf("is sleeping", philo);
	if (opt_sleep(philo->data->t_sleep, philo->data))
		return 1;
	return (0);
}

int	thinking(philo_t *philo)
{
	if (stats(philo))
		return (1);
	my_printf("is thinking", philo);
	usleep(250);
	return (0);
}

void	*one_philo(philo_t *philo)
{
	printf("1  1 has taken a fork\n");
	opt_sleep(philo->data->t_die, philo->data);
	return (NULL);
}

void	*routine(void *ph)
{
	philo_t	*philo;

	philo = (void *)ph;
	if (philo->data->n_philo == 1)
		return (one_philo(philo));
	while(1)
	{
		pthread_mutex_lock(&philo->eating_times);
		if (philo->n_eats == philo->data->noft_eat)
		{
			
			pthread_mutex_lock(&philo->data->finished_mutex);
			philo->data->finished++;
			pthread_mutex_unlock(&philo->data->finished_mutex);
			pthread_mutex_unlock(&philo->eating_times);
			break;
		}
		philo->n_eats++;
		pthread_mutex_unlock(&philo->eating_times);
		if (eating(philo) || sleeping(philo) || thinking(philo))
			break;
	}
	return (NULL);
}
