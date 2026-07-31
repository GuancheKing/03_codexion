/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:56:06 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/31 13:18:55 by josjimen         ###   ########.fr       */
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
