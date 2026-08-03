/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 09:25:28 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 18:18:22 by josjimen         ###   ########.fr       */
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

static void	destroy_coders(t_simulation *simulation)
{
	int	i;

	if (simulation->coders == NULL)
		return ;
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->coders[i].state_mutex);
		i++;
	}
	free(simulation->coders);
	simulation->coders = NULL;
}

void	destroy_simulation(t_simulation *simulation)
{
	if (simulation->queue.items != NULL)
		destroy_queue(simulation);
	if (simulation->dongles != NULL)
		destroy_dongles(simulation);
	if (simulation->coders != NULL)
		destroy_coders(simulation);
	pthread_mutex_destroy(&simulation->log_mutex);
	pthread_mutex_destroy(&simulation->finish_mutex);
	pthread_cond_destroy(&simulation->start_condition);
	pthread_mutex_destroy(&simulation->start_mutex);
}
