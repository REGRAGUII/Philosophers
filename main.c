/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:41:09 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/09 04:27:45 by yregragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void	*routine()
{
	// int i;
	// for(i= 0; i < 10000000; i++)
	// {
		// m++;
	// }
}

void	run_threads(philo_t *philo)
{
	pthread_t	th[philo->N_philo];
	int			i;

	i = 0;
	while (i < philo->N_philo)
	{
		pthread_create(&th[i], NULL, &routine, NULL);
		printf("thread number %i created\n", i);
		i++;
	}
	i = 0;
	while(i < philo->N_philo)
	{
		pthread_join(th[i], NULL);
 		printf("thread number %i finished\n", i);
		i++;
	}
}


int main(int ac, char **av)
{
	philo_t *philo;

	if(!check_args(av, ac, philo))
	{
		philo = malloc(sizeof(philo_t));
		philo->N_philo = ft_atoi(av[1]);
		philo->T_die = ft_atoi(av[2]);
    	philo->T_eat = ft_atoi(av[3]);
    	philo->T_sleep = ft_atoi(av[4]);
		if(ac == 6)
        	philo->NofT_Eat = ft_atoi(av[5]);
		else
			philo->NofT_Eat = 0;
		// printf("struct: %i", philo->N_philo);
    	// printf("struct: %i", philo->T_die);
    	// printf("struct: %i", philo->T_eat);
    	// printf("struct: %i", philo->T_sleep);
    	// printf("struct: %i\n", philo->NofT_Eat);
		run_threads(philo);
		free(philo);
	}
	return (1);
}