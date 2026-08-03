/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 06:55:36 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 20:11:41 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_sync(t_simulation *simulation)
{
	if (pthread_mutex_init(&simulation->start_mutex, NULL) != 0)
		return (1);
	if (pthread_cond_init(&simulation->start_condition, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->start_mutex);
		return (1);
	}
	if (pthread_mutex_init(&simulation->finish_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->start_condition);
		pthread_mutex_destroy(&simulation->start_mutex);
		return (1);
	}
	if (pthread_mutex_init(&simulation->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->finish_mutex);
		pthread_cond_destroy(&simulation->start_condition);
		pthread_mutex_destroy(&simulation->start_mutex);
		return (1);
	}
	return (0);
}

int	start_simulation(t_simulation *simulation)
{
	int		i;

	i = 0;
	pthread_mutex_lock(&simulation->start_mutex);
	simulation->simulation_start = get_time_ms();
	if (simulation->simulation_start == -1)
	{
		pthread_mutex_unlock(&simulation->start_mutex);
		set_simulation_finished(simulation);
		pthread_mutex_lock(&simulation->start_mutex);
		simulation->start_ready = true;
		pthread_cond_broadcast(&simulation->start_condition);
		pthread_mutex_unlock(&simulation->start_mutex);
		return (1);
	}
	while (i < simulation->created_threads)
	{
		simulation->coders[i].last_compile_time
			= simulation->simulation_start;
		i++;
	}
	simulation->start_ready = true;
	pthread_cond_broadcast(&simulation->start_condition);
	pthread_mutex_unlock(&simulation->start_mutex);
	return (0);
}

int	init_simulation(int argc, char **argv, t_simulation *simulation)
{
	if (parser(argc, argv, &simulation->config) == 1)
		return (1);
	init_default_values(simulation);
	if (init_sync(simulation) == 1)
		return (1);
	if (init_resources(simulation) == 1)
		return (1);
	return (0);
}
