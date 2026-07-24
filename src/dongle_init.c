/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 14:24:18 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/24 09:44:15 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static void	cleanup_initialized_dongles(
	t_simulation *simulation, int initialized_count)
{
	while (initialized_count > 0)
	{
		initialized_count--;
		pthread_mutex_destroy(
			&simulation->dongles[initialized_count].dongle_mutex);
	}
	free(simulation->dongles);
	simulation->dongles = NULL;
}

void	assign_dongles(t_simulation *simulation)
{
	int	i;
	int	total;

	i = 0;
	total = simulation->config.number_of_coders;
	while (i < total)
	{
		simulation->coders[i].left_dongle = &simulation->dongles[i];
		simulation->coders[i].right_dongle
			= &simulation->dongles[(i + 1) % total];
		i++;
	}
}

int	init_dongles(t_simulation *simulation)
{
	int	i;

	simulation->dongles = malloc(
			simulation->config.number_of_coders
			* sizeof(*simulation->dongles));
	if (simulation->dongles == NULL)
		return (1);
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->dongles[i].id = (i + 1);
		simulation->dongles[i].available_at = 0;
		simulation->dongles[i].in_use = false;
		if (pthread_mutex_init(&simulation->dongles[i].dongle_mutex, NULL) != 0)
		{
			cleanup_initialized_dongles(simulation, i);
			return (1);
		}
		i++;
	}
	return (0);
}
