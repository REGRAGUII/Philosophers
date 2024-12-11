/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/11 06:10:01 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void	*routine()
{
	
}

void	run_threads(data_t *data, philo_t *philo)
{
	int			i;

	i = 0;
	while (i < data->N_philo)
	{
		pthread_create(&philo->thread, NULL, &routine, NULL);
		printf("thread number %i created\n", i);
		i++;
	}
	i = 0;
	while(i < data->N_philo)
	{
		pthread_join(philo->thread, NULL);
 		printf("thread number %i finished\n", i);
		i++;
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
void	ft_init(data_t *data, philo_t *philo, int ac, char **av)
{
	philo_t *new_philo;
	philo_t	*tmp;
	int		i;
	
	data->N_philo = ft_atoi(av[1]);
	data->T_die = ft_atoi(av[2]);
    data->T_eat = ft_atoi(av[3]);
    data->T_sleep = ft_atoi(av[4]);
	if(ac == 6)
    	data->NofT_Eat = ft_atoi(av[5]);
	else
		data->NofT_Eat = 0;
	pthread_mutex_init(&philo->left_fork, NULL);
	pthread_mutex_init(&philo->right_fork, NULL);
	philo->last_meal = 0;
	philo->id = 1;
	philo->left = NULL;
	philo->right = NULL;
	philo->data = data;
	i = 2;
	while (i <= data->N_philo)
	{
		new_philo = malloc(sizeof(philo_t));
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
	

}	

int main(int ac, char **av)
{
	data_t *data;
	philo_t	*philo;


	if(!check_args(av, ac))
	{
		data = malloc(sizeof(data_t));
		ft_init(data, philo, ac, av);
		run_threads(data, philo);
		free(data);
	}
	return (1);
}