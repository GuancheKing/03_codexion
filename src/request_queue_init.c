/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_queue_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:50:54 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/24 16:36:02 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	init_request_queue(t_simulation *simulation)
{
	simulation->queue.capacity = simulation->config.number_of_coders;
	simulation->queue.items = malloc(
			simulation->queue.capacity * sizeof(
				*simulation->queue.items));
	if (simulation->queue.items == NULL)
		return (1);
	simulation->queue.size = 0;
	simulation->queue.next_arrival_order = 0;
	if (pthread_mutex_init(&simulation->queue.queue_mutex, NULL) != 0)
	{
		free(simulation->queue.items);
		simulation->queue.items = NULL;
		return (1);
	}
	if (pthread_cond_init(&simulation->queue.queue_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->queue.queue_mutex);
		free(simulation->queue.items);
		simulation->queue.items = NULL;
		return (1);
	}
	return (0);
}
