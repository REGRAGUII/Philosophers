/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yregragu <yregragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:51:16 by yregragu          #+#    #+#             */
/*   Updated: 2024/12/08 23:47:42 by yregragu         ###   ########.fr       */
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


int	check_args(char **av, int ac, philo_t *philo)
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
        printf("her : %s\n", av[i]);
		if(ft_atoi(av[i]) < 0)
        {
            printf("%s", ERROR_ARGS);
            return(1);
        }
        i++;
	}

	return(0);
}
