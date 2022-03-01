/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amorion- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 09:57:42 by amorion-          #+#    #+#             */
/*   Updated: 2021/10/25 09:57:46 by amorion-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_routine(void *philos)
{ 
	t_philo *philo;

	philo = (t_philo *)philos;
	philo->t_meal = philo->args->t0;
	while(!philo->args->end)
	{
		if(take_fork(philo))
			philo->args->end = 1;
		else
		{
			pthread_mutex_lock(&philo->m_philo);
			ft_print(philo, 1);
			philo->n_meals++;
			gettimeofday(&philo->t_meal, NULL);
			ft_wait(philo->args->eat_time * 1000, philo->args);
			pthread_mutex_unlock(&philo->m_philo);
			release_fork(philo);
			ft_print(philo, 2);
			ft_wait(philo->args->sleep_time * 1000, philo->args);
			ft_print(philo, 3);
			usleep(100/philo->args->nphilo);
		}
	}
	return(0);
}
void	ft_philo_init(t_philo *philo, int pos, pthread_mutex_t *forks, t_args *args)
{
	philo->num = pos + 1;
	philo->args = args;
	philo->n_meals = 0;
	philo->forks[0] = &forks[pos];
	philo->forks[1] = &forks[philo->num % args->nphilo];
	pthread_mutex_init(&philo->m_philo, NULL);
	gettimeofday(&philo->t_meal, NULL);
}

void	ft_philosophers(t_args *args)
{
	int i;
	t_philo philo[args->nphilo];
	pthread_t philos[args->nphilo];
	args->end = 0;
	i = 0;
	gettimeofday(&args->t0, NULL);
	while(i < args->nphilo)
	{
		ft_philo_init(&philo[i], i, args->forks, args);
		i++;
	}
	i = 0;
	while(i < args->nphilo)
	{
		pthread_create(&philos[i], 0, ft_routine, &philo[i]);
		i++;
	}
	death_meals(args, philo);
	printf("OK\n");
	i = 0;
	while (i < args->nphilo)
	{
		pthread_join(philos[i], NULL);
		i++;
	}
}