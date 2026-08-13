/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_conflict.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 07:15:13 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/13 15:10:23 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coders_share_dongle(t_coder *coder_a, t_coder *coder_b)
{
	if (coder_a->left_dongle == coder_b->left_dongle)
		return (true);
	if (coder_a->left_dongle == coder_b->right_dongle)
		return (true);
	if (coder_a->right_dongle == coder_b->left_dongle)
		return (true);
	if (coder_a->right_dongle == coder_b->right_dongle)
		return (true);
	return (false);
}

static t_request	*find_coder_request(t_request_queue *queue, t_coder *coder)
{
	int			i;
	t_request	*my_request;

	i = 0;
	my_request = NULL;
	while (i < queue->size)
	{
		if (queue->items[i].coder == coder)
		{
			my_request = &queue->items[i];
			break ;
		}
		i++;
	}
	return (my_request);
}

static bool	other_request_has_priority(
	t_request *my_request, t_request *other_request,
	t_scheduler scheduler, long current_time
)
{
	bool	has_busy_dongle;
	long	available_at;

	if (my_request->coder == other_request->coder)
		return (false);
	if (!coders_share_dongle(my_request->coder, other_request->coder))
		return (false);
	if (!request_has_priority(other_request, my_request, scheduler))
		return (false);
	get_coder_dongles_wait_info(
		other_request->coder,
		&has_busy_dongle,
		&available_at
		);
	if (has_busy_dongle)
		return (false);
	if (available_at > current_time)
		return (false);
	return (true);
}

bool	request_has_dongle_priority(
	t_request_queue *queue, t_coder *coder, t_scheduler scheduler
)
{
	int			i;
	t_request	*my_request;
	t_request	*other_request;
	long		current_time;

	my_request = find_coder_request(queue, coder);
	if (my_request == NULL)
		return (false);
	current_time = get_time_ms();
	if (current_time == -1)
		return (false);
	i = 0;
	while (i < queue->size)
	{
		other_request = &queue->items[i];
		if (other_request_has_priority(
				my_request,
				other_request,
				scheduler,
				current_time
			))
			return (false);
		i++;
	}
	return (true);
}

int	find_coder_request_index(t_request_queue *queue, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < queue->size)
	{
		if (queue->items[i].coder == coder)
			return (i);
		i++;
	}
	return (-1);
}
