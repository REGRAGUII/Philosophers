/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/16 18:45:28 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

bool eating(philo_t *philo)
{
	// pthread_mutex_lock(&philo->fork);
	printf("%d is eating\n", philo->id);
	philo->eating_times--;
	pthread_mutex_lock(&philo->data->end_mutex);
	if (philo->eating_times == 0)
		philo->data->end = 1;
	pthread_mutex_unlock(&philo->data->end_mutex);
}

void	sleeping(philo_t *philo)
{
	printf("%d is sleeping\n", philo->id);
	
}

void	thinking(philo_t *philo)
{
	printf("%d is thinking\n", philo->id);
}


void	*routine(void *ph)
{
	philo_t	*philo;
	
	philo = (void *)ph;
	if(philo->id % 2 == 0)
		usleep(400);
	while(1)
	{
		pthread_mutex_lock(&philo->data->end_mutex);
		if(!philo->data->end)
		{
			pthread_mutex_unlock(&philo->data->end_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->end_mutex);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
}

void	run_threads(data_t *data, philo_t *philo)
{
	int			i;
	philo_t		*head;

	head = philo;
	i = 1;
	while (i <= data->N_philo)
	{
		pthread_create(&philo->thread, NULL, &routine, philo);
		printf("thread number %i created\n", i);
		i++;
		philo = philo->right;
	}
	head = philo;
	i = 1;
	while(i <= data->N_philo)
	{
		pthread_join(philo->thread, NULL);
 		printf("thread number %i finished\n", i);
		i++;
		philo = philo->right;
	}
}

void	ft_lstadd_back(philo_t *lst, philo_t *new)
{
	philo_t	*head;

	head = lst;
	if(!new)
		return ;
	if(!lst)
		lst = new;
	else
	{
		while (head->right)
			head = head->right;
		head->right = new;
		new->right = NULL;
	}
}
philo_t	*ft_init(data_t *data, philo_t *philo, int ac, char **av)
{
	philo_t *new_philo;
	philo_t	*tmp;
	philo_t *head;
	int		i;
	
	data->N_philo = ft_atoi(av[1]);
	data->T_die = ft_atoi(av[2]);
	data->T_eat = ft_atoi(av[3]);
	data->T_sleep = ft_atoi(av[4]);
	data->end = -1;
	pthread_mutex_init(&data->end_mutex, NULL);
	if(ac == 6)
		data->NofT_Eat = ft_atoi(av[5]);
	else
		data->NofT_Eat = 0;
	philo = malloc(sizeof(philo_t));
	head = philo;
	pthread_mutex_init(&philo->fork, NULL);
	philo->eating_times = data->N_philo;
	philo->last_meal = 0;
	philo->id = 1;
	philo->left = NULL;
	philo->right = NULL;
	philo->data = data;
	i = 2;
	while (i <= data->N_philo)
	{
		new_philo = malloc(sizeof(philo_t));
		pthread_mutex_init(&new_philo->fork, NULL);
		new_philo->right = NULL;
		new_philo->left = NULL;
		new_philo->id = i;
		new_philo->eating_times = data->NofT_Eat;
		new_philo->data = data; 
		ft_lstadd_back(philo, new_philo);
		tmp = philo;
		philo = philo->right;
		philo->left = tmp;
		i++;
	}
	philo->right = head;
	head->left = philo;
	return (head);
}

int main(int ac, char **av)
{
	data_t *data;
	philo_t	*philo;
	philo_t	*head;
	
	if(!check_args(av, ac))
	{
		data = malloc(sizeof(data_t));
		head = philo;
		run_threads(data, ft_init(data, philo, ac, av));
		return (0);
		free(data);
	}
	return (1);
}