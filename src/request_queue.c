/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:18:39 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/05 22:52:10 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	enqueue_coder_request(t_coder *coder)
{
	t_simulation	*simulation;
	t_request		request;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->queue.queue_mutex);
	request.coder = coder;
	request.arrival_order = simulation->queue.next_arrival_order;
	request.deadline
		= get_last_compile_time(coder) + simulation->config.time_to_burnout;
	if (push_request(
			&simulation->queue,
			request,
			simulation->config.scheduler
		) == 1)
	{
		pthread_mutex_unlock(&simulation->queue.queue_mutex);
		return (1);
	}
	simulation->queue.next_arrival_order++;
	pthread_cond_broadcast(&simulation->queue.queue_cond);
	pthread_mutex_unlock(&simulation->queue.queue_mutex);
	return (0);
}

bool	coder_is_queue_head(t_request_queue *queue, t_coder *coder)
{
	if (
		(queue->size > 0)
		&& (queue->items[0].coder == coder)
	)
		return (true);
	return (false);
}
