/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 06:55:36 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 14:39:02 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	init_simulation(int argc, char **argv, t_simulation *simulation)
{
	if (parser(argc, argv, &simulation->config) == 1)
		return (1);
	simulation->simulation_finished = false;
	simulation->created_threads = 0;
	if (pthread_mutex_init(&simulation->finish_mutex, NULL) != 0)
		return (1);
	simulation->coders = malloc(
			simulation->config.number_of_coders
			* sizeof(*simulation->coders));
	if (simulation->coders == NULL)
	{
		destroy_simulation(simulation);
		return (1);
	}
	simulation->simulation_start = get_time_ms();
	if (simulation->simulation_start == -1)
		return (1);
	init_coders(simulation);
	return (0);
}

void	init_coders(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].completed_compiles = 0;
		simulation->coders[i].last_compile_time = simulation->simulation_start;
		simulation->coders[i].simulation = simulation;
		i++;
	}
}
