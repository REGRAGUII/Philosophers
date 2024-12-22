/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/22 19:11:53 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"


void my_printf(char *msg, philo_t *philo)
{
	long time;

	time = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->end_mutex);
	if(philo->data->end == 0)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		pthread_mutex_unlock(&philo->data->print);
		return ;
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	printf("%ld  %d  %s\n",time,  philo->id, msg);
	pthread_mutex_unlock(&philo->data->print);
}

int stats(philo_t *philo)
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
int eating(philo_t *philo)
{
	if (stats(philo))
		return (1);
	if(philo->id %2 == 0)
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
	
	if (opt_sleep(philo->data->T_eat, philo->data))
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);		
		return (1);
	}
	// if(philo->id % 2 == 0)
	// {
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);
	// }
	// else
	// {
	// 	pthread_mutex_unlock(&philo->left->fork);
	// 	pthread_mutex_unlock(&philo->fork);
	// }
	// return (0);
}

int	sleeping(philo_t *philo)
{
	long	time;

	if (stats(philo))
	{
		return (1);
	}
	my_printf("is sleeping", philo);	
	if (opt_sleep(philo->data->T_sleep, philo->data))
		return 1;
}

int	thinking(philo_t *philo)
{
	long	time;
	if (stats(philo))
	{
		return (1);
	}
	my_printf("is thinking", philo);
	usleep(250);
}


void	*routine(void *ph)
{
	philo_t	*philo;

	philo = (void *)ph;
	if (philo->data->N_philo == 1)
	{
		printf("1  1 has taken a fork\n");
		opt_sleep(philo->data->T_die, philo->data);
		return NULL;
	}
	while(1)
	{
		pthread_mutex_lock(&philo->eating_times);
		if (philo->n_eats == philo->data->NofT_Eat)
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
		i++;
		philo = philo->right;
	}
	pthread_create(&philo->data->checker, NULL, &checker, philo);
	i = 1;
	while(i <= data->N_philo)
	{
		pthread_join(philo->thread, NULL);
		philo = philo->right;
		i++;
	}
	pthread_join(philo->data->checker, NULL);
}

philo_t *ft_init1(philo_t *philo, data_t *data, int i)
{
	philo_t *new_philo;
	philo_t	*tmp;
	philo_t *head;
	
	head = philo;
	while (++i <= data->N_philo)
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
	return(head);
}

philo_t	*ft_init(data_t *data, philo_t *philo, int ac, char **av)
{
	data->N_philo = ft_atoi(av[1]);
	data->T_die = ft_atoi(av[2]);
	data->T_eat = ft_atoi(av[3]);
	data->T_sleep = ft_atoi(av[4]);
	data->end = -1;
	data->start_time = get_time();
	if(ac == 6)
		data->NofT_Eat = ft_atoi(av[5]);
	else
		data->NofT_Eat = -1;
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

int main(int ac, char **av)
{
	data_t *data;
	philo_t	*philo;
	philo_t	*head;
	
	if(!check_args(av, ac))
	{
		data = malloc(sizeof(data_t));
		philo = malloc(sizeof(philo_t));
		run_threads(data, ft_init(data, philo, ac, av));
		clean(data, philo);
		return (0);
	}
	return (1);
}