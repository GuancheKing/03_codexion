/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_wait_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 07:46:38 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/13 09:03:39 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_claim_result	try_claim_request(t_coder *coder)
{
	t_simulation	*simulation;
	long			current_time;

	simulation = coder->simulation;
	current_time = get_time_ms();
	if (current_time == -1)
		return (CLAIM_ERROR);
	if (try_reserve_dongles(coder, current_time) == false)
		return (CLAIM_WAIT);
	if (remove_coder_request(
			&simulation->queue,
			coder,
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

static t_claim_result	handle_request_claim(
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

int	handle_request_turn(t_coder *coder, int *wait_result)
{
	t_simulation	*simulation;
	t_claim_result	result;
	int				action;

	simulation = coder->simulation;
	if (!request_has_dongle_priority(&simulation->queue, coder,
			simulation->config.scheduler))
	{
		*wait_result = pthread_cond_wait(
				&simulation->queue.queue_cond,
				&simulation->queue.queue_mutex);
		return (-1);
	}
	result = handle_request_claim(coder, wait_result);
	action = process_head_result(simulation, result);
	return (action);
}
