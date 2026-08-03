/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 19:55:55 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 20:11:41 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	init_default_values(t_simulation *simulation)
{
	simulation->start_ready = false;
	simulation->simulation_finished = false;
	simulation->created_threads = 0;
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->queue.items = NULL;
}

static int	init_coders(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].completed_compiles = 0;
		simulation->coders[i].last_compile_time = 0;
		simulation->coders[i].simulation = simulation;
		if (pthread_mutex_init(&simulation->coders[i].state_mutex, NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&simulation->coders[i].state_mutex);
			}
			return (1);
		}
		i++;
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

int	init_resources(t_simulation *simulation)
{
	if (allocate_coders(simulation) == 1)
		return (1);
	if (init_coders(simulation) != 0)
	{
		free(simulation->coders);
		simulation->coders = NULL;
		destroy_simulation(simulation);
		return (1);
	}
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
