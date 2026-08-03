/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 06:55:36 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 13:05:34 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static void	init_coders(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].completed_compiles = 0;
		simulation->coders[i].last_compile_time = 0;
		simulation->coders[i].simulation = simulation;
		i++;
	}
}

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
	return (0);
}

static int	allocate_coders(t_simulation *simulation)
{
	simulation->coders = malloc(
			simulation->config.number_of_coders
			* sizeof(*simulation->coders));
	if (simulation->coders == NULL)
	{
		destroy_simulation(simulation);
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
	simulation->start_ready = false;
	simulation->simulation_finished = false;
	simulation->created_threads = 0;
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->queue.items = NULL;
	if (init_sync(simulation) == 1)
		return (1);
	if (allocate_coders(simulation) == 1)
		return (1);
	init_coders(simulation);
	if (init_dongles(simulation) == 1)
	{
		destroy_simulation(simulation);
		return (1);
	}
	assign_dongles(simulation);
	if (init_request_queue(simulation) == 1)
	{
		destroy_simulation(simulation);
		return (1);
	}
	return (0);
}
