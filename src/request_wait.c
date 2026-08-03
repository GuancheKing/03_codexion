/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 12:27:15 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 13:05:34 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>
#include <errno.h>

static int	process_head_result(
	t_simulation	*simulation,
	t_claim_result	result
)
{
	if (result == CLAIM_SUCCESS)
	{
		pthread_mutex_unlock(&simulation->queue.queue_mutex);
		return (0);
	}
	if (result == CLAIM_ERROR)
		return (stop_dongle_wait(simulation));
	return (-1);
}

static t_claim_result	try_claim_request(t_coder *coder)
{
	t_simulation	*simulation;
	t_request		request;
	long			current_time;

	simulation = coder->simulation;
	current_time = get_time_ms();
	if (current_time == -1)
		return (CLAIM_ERROR);
	if (try_reserve_dongles(coder, current_time) == false)
		return (CLAIM_WAIT);
	if (pop_request(
			&simulation->queue,
			&request,
			simulation->config.scheduler) == 1)
	{
		cancel_dongle_reservation(coder);
		pthread_cond_broadcast(&simulation->queue.queue_cond);
		return (CLAIM_ERROR);
	}
	pthread_cond_broadcast(&simulation->queue.queue_cond);
	return (CLAIM_SUCCESS);
}

static int	wait_for_queue_or_cooldown(t_coder *coder)
{
	t_simulation	*simulation;
	bool			busy;
	long			available_at;
	struct timespec	timeout;

	simulation = coder->simulation;
	get_coder_dongles_wait_info(coder, &busy, &available_at);
	if (busy == true)
		return (pthread_cond_wait(
				&simulation->queue.queue_cond,
				&simulation->queue.queue_mutex));
	timeout.tv_sec = available_at / 1000;
	timeout.tv_nsec = (available_at % 1000) * 1000000;
	return (pthread_cond_timedwait(
			&simulation->queue.queue_cond,
			&simulation->queue.queue_mutex,
			&timeout));
}

static t_claim_result	handle_queue_head(
	t_coder *coder,
	int *wait_result
)
{
	t_claim_result	result;

	result = try_claim_request(coder);
	if (result == CLAIM_WAIT)
		*wait_result = wait_for_queue_or_cooldown(coder);
	return (result);
}

int	wait_for_dongles(t_coder *coder)
{
	t_simulation	*simulation;
	t_claim_result	result;
	int				wait_result;
	int				action;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->queue.queue_mutex);
	while (is_simulation_finished(simulation) == false)
	{
		if (coder_is_queue_head(&simulation->queue, coder))
		{
			result = handle_queue_head(coder, &wait_result);
			action = process_head_result(simulation, result);
			if (action != -1)
				return (action);
		}
		else
			wait_result = pthread_cond_wait(
					&simulation->queue.queue_cond,
					&simulation->queue.queue_mutex);
		if ((wait_result != 0) && (wait_result != ETIMEDOUT))
			return (stop_dongle_wait(simulation));
	}
	pthread_mutex_unlock(&simulation->queue.queue_mutex);
	return (1);
}
