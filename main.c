/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/21 23:40:02 by yregragu         ###   ########.fr       */
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

int my_printf(char *msg, philo_t *philo, int stats)
{
	long time;

	pthread_mutex_lock(&philo->data->print);
	if(stats = 1)
		philo->n_eats--;
	time = get_time() - philo->data->start_time;
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
int eating(philo_t *philo)
{
	if (stats(philo))
		return (1);

	if(philo->id %2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->left->fork);
	}
	else
	{
		pthread_mutex_lock(&philo->left->fork);
		pthread_mutex_lock(&philo->fork);
	}
	
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	
	my_printf("has taken a fork", philo, 0);
	my_printf("is eating", philo, 1);
	

	if (opt_sleep(philo->data->T_eat, philo->data))
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);		
		return (1);
	}

	if(philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->left->fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->left->fork);
		pthread_mutex_unlock(&philo->fork);
	}
	return (0);
}

int	sleeping(philo_t *philo)
{
	long	time;

	if (stats(philo))
	{
		return (1);
	}
	my_printf("is sleeping", philo, 0);	
	if (opt_sleep(philo->data->T_sleep, philo->data))
		return 1;
}

int	thinking(philo_t *philo)
{
	long	time;
	//
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->end)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		return (1);
	}
	//
	pthread_mutex_unlock(&philo->data->end_mutex);
	my_printf("is thinking", philo, 0);
	usleep(250);
}


void	*routine(void *ph)
{
	philo_t	*philo;
	int eats;

	eats = 0;
	
	philo = (void *)ph;
	if (philo->data->N_philo == 1)
	{
		printf("1  1 has taken a fork\n");
		opt_sleep(philo->data->T_die, philo->data);
		return NULL;
	}
	while(1)
	{
		pthread_mutex_lock(&philo->data->eats_mutex);
		if (eats == philo->data->NofT_Eat)
		{
			pthread_mutex_unlock(&philo->data->eats_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->eats_mutex);
		
		if (eating(philo))
			break;
		if (sleeping(philo))
			break;
		if(thinking(philo))
			break;
		eats++;
	}
}

int	is_dead(philo_t *philo)
{
	// long	time;

	// time = get_time();
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

	if (is_dead(philo))
	{
		my_printf("died", philo, 0);
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
	data->eats = -1;
	data->start_time = get_time();
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->eats_mutex, NULL);
	pthread_mutex_init(&data->time_mutex, NULL);
	if(ac == 6)
		data->NofT_Eat = ft_atoi(av[5]);
	else
		data->NofT_Eat = -1;
	head = philo;
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->eating_times, NULL);
	pthread_mutex_init(&philo->last_meal_mutex, NULL);
	philo->last_meal = get_time();
	philo->id = 1;
	philo->left = NULL;
	philo->right = NULL;
	philo->data = data;
	i = 2;
	while (i <= data->N_philo)
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
		philo = malloc(sizeof(philo_t));
		run_threads(data, ft_init(data, philo, ac, av));
		clean(data, philo);
		return (0);
	}
	return (1);
}