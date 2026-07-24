/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 09:25:28 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/24 17:12:40 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static void	destroy_queue(t_simulation *simulation)
{
	pthread_cond_destroy(&simulation->queue.queue_cond);
	pthread_mutex_destroy(&simulation->queue.queue_mutex);
	free(simulation->queue.items);
	simulation->queue.items = NULL;
}

static void	destroy_dongles(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->dongles[i].dongle_mutex);
		i++;
	}
	free(simulation->dongles);
	simulation->dongles = NULL;
}

void	destroy_simulation(t_simulation *simulation)
{
	if (simulation->queue.items != NULL)
		destroy_queue(simulation);
	if (simulation->dongles != NULL)
		destroy_dongles(simulation);
	free(simulation->coders);
	simulation->coders = NULL;
	pthread_mutex_destroy(&simulation->finish_mutex);
	pthread_cond_destroy(&simulation->start_condition);
	pthread_mutex_destroy(&simulation->start_mutex);
}
