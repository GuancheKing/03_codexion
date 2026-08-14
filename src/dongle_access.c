/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:56:06 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/05 16:43:36 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	choose_dongle_order(
	t_coder *coder,
	t_dongle **first,
	t_dongle **second
)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

bool	try_reserve_dongles(t_coder *coder, long current_time)
{
	t_dongle	*first;
	t_dongle	*second;
	bool		reserved;

	choose_dongle_order(coder, &first, &second);
	reserved = false;
	if (first == second)
		return (reserved);
	pthread_mutex_lock(&first->dongle_mutex);
	pthread_mutex_lock(&second->dongle_mutex);
	if (
		first->in_use == false
		&& second->in_use == false
		&& first->available_at <= current_time
		&& second->available_at <= current_time
	)
	{
		first->in_use = true;
		second->in_use = true;
		reserved = true;
	}
	pthread_mutex_unlock(&second->dongle_mutex);
	pthread_mutex_unlock(&first->dongle_mutex);
	return (reserved);
}

void	release_dongles(t_coder *coder, long release_time)
{
	t_dongle	*first;
	t_dongle	*second;
	long		cooldown;

	pthread_mutex_lock(&coder->simulation->queue.queue_mutex);
	cooldown = coder->simulation->config.dongle_cooldown;
	choose_dongle_order(coder, &first, &second);
	if (first == second)
	{
		pthread_mutex_unlock(&coder->simulation->queue.queue_mutex);
		return ;
	}
	pthread_mutex_lock(&first->dongle_mutex);
	pthread_mutex_lock(&second->dongle_mutex);
	first->in_use = false;
	second->in_use = false;
	first->available_at = safe_time_add(release_time, cooldown);
	second->available_at = safe_time_add(release_time, cooldown);
	pthread_mutex_unlock(&second->dongle_mutex);
	pthread_mutex_unlock(&first->dongle_mutex);
	pthread_cond_broadcast(&coder->simulation->queue.queue_cond);
	pthread_mutex_unlock(&coder->simulation->queue.queue_mutex);
}

void	cancel_dongle_reservation(t_coder	*coder)
{
	t_dongle	*first;
	t_dongle	*second;

	choose_dongle_order(coder, &first, &second);
	if (first == second)
		return ;
	pthread_mutex_lock(&first->dongle_mutex);
	pthread_mutex_lock(&second->dongle_mutex);
	first->in_use = false;
	second->in_use = false;
	pthread_mutex_unlock(&second->dongle_mutex);
	pthread_mutex_unlock(&first->dongle_mutex);
}

void	get_coder_dongles_wait_info(
	t_coder *coder,
	bool *has_busy_dongle,
	long *available_at
)
{
	t_dongle	*first;
	t_dongle	*second;

	choose_dongle_order(coder, &first, &second);
	if (first == second)
	{
		*has_busy_dongle = true;
		*available_at = 0;
		return ;
	}
	pthread_mutex_lock(&first->dongle_mutex);
	pthread_mutex_lock(&second->dongle_mutex);
	*has_busy_dongle = (first->in_use || second->in_use);
	if (first->available_at > second->available_at)
		*available_at = first->available_at;
	else
		*available_at = second->available_at;
	pthread_mutex_unlock(&second->dongle_mutex);
	pthread_mutex_unlock(&first->dongle_mutex);
}
