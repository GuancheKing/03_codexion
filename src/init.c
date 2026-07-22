/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 06:55:36 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 10:12:55 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	init_simulation(int argc, char **argv, t_simulation *simulation)
{
	if (parser(argc, argv, &simulation->config) == 1)
		return (1);
	simulation->simulation_start = get_time_ms();
	if (simulation->simulation_start == -1)
		return (1);
	simulation->simulation_finished = false;
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
	return (0);
}
