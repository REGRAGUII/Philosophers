/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:51:16 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/21 23:03:44 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	long	n;
	int     x;

	x = 0;
	n = 0;
	while (str[x] >= '0' && str[x] <= '9')
	{
		n = n * 10 + str[x] - '0';
		x++;
	}
	if (str[x] || n > INT_MAX)
		return (-1);
	return ((int)n);
}


int	check_args(char **av, int ac)
{
	int	i;

	i = 1;
    if(ac < 5 || ac > 6)
	{
		printf("%s\n", ERROR_ARGS);
		return(1);
	}
	while(i < ac)
	{
		if(ft_atoi(av[i]) < 0)
        {
            printf("%s", ERROR_ARGS);
            return(1);
        }
        i++;
	}

	return(0);
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


void clean(data_t *data, philo_t *philo)
{
	philo_t	*tmp;
	int 	i;

	tmp = philo;
	i = data->N_philo;
	while(i > 0)
	{
		pthread_mutex_destroy(&philo->fork);
		pthread_mutex_destroy(&philo->eating_times);
		tmp = philo;
		philo = philo->right;
		free(tmp);
		i--;
	}
	pthread_mutex_destroy(&data->eats_mutex);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->time_mutex);
	free(data);
}