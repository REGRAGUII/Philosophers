/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/22 20:10:32 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	stats(philo_t *philo)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->end)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	return (0);
}

void	run_threads(data_t *data, philo_t *philo)
{
	int			i;

	i = 1;
	while (i <= data->n_philo)
	{
		pthread_create(&philo->thread, NULL, &routine, philo);
		i++;
		philo = philo->right;
	}
	pthread_create(&philo->data->checker, NULL, &checker, philo);
	i = 1;
	while (i <= data->n_philo)
	{
		pthread_join(philo->thread, NULL);
		philo = philo->right;
		i++;
	}
	pthread_join(philo->data->checker, NULL);
}

philo_t	*ft_init1(philo_t *philo, data_t *data, int i)
{
	philo_t	*new_philo;
	philo_t	*tmp;
	philo_t	*head;

	head = philo;
	while (++i <= data->n_philo)
	{
		new_philo = malloc(sizeof(philo_t));
		pthread_mutex_init(&new_philo->fork, NULL);
		pthread_mutex_init(&new_philo->eating_times, NULL);
		pthread_mutex_init(&new_philo->last_meal_mutex, NULL);
		new_philo->right = NULL;
		new_philo->left = NULL;
		new_philo->id = i;
		new_philo->last_meal = get_time();
		new_philo->data = data;
		new_philo->n_eats = 0;
		ft_lstadd_back(philo, new_philo);
		tmp = philo;
		philo = philo->right;
		philo->left = tmp;
	}
	philo->right = head;
	head->left = philo;
	return (head);
}

philo_t	*ft_init(data_t *data, philo_t *philo, int ac, char **av)
{
	data->n_philo = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	data->end = -1;
	data->start_time = get_time();
	if (ac == 6)
		data->noft_eat = ft_atoi(av[5]);
	else
		data->noft_eat = -1;
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->time_mutex, NULL);
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->eating_times, NULL);
	pthread_mutex_init(&data->finished_mutex, NULL);
	pthread_mutex_init(&philo->last_meal_mutex, NULL);
	data->finished = 0;
	philo->last_meal = get_time();
	philo->n_eats = 0;
	philo->id = 1;
	philo->left = NULL;
	philo->right = NULL;
	philo->data = data;
	return (ft_init1(philo, data, 1));
}

int	main(int ac, char **av)
{
	data_t	*data;
	philo_t	*philo;

	if (!check_args(av, ac))
	{
		data = malloc(sizeof(data_t));
		philo = malloc(sizeof(philo_t));
		run_threads(data, ft_init(data, philo, ac, av));
		clean(data, philo);
		return (0);
	}
	return (1);
}
