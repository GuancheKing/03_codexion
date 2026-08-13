/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 12:27:15 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/13 09:03:38 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>
#include <errno.h>

int	wait_for_dongles(t_coder *coder)
{
	t_simulation	*simulation;
	int				wait_result;
	int				action;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->queue.queue_mutex);
	while (is_simulation_finished(simulation) == false)
	{
		action = handle_request_turn(coder, &wait_result);
		if (action != -1)
			return (action);
		if ((wait_result != 0) && (wait_result != ETIMEDOUT))
			return (stop_dongle_wait(simulation));
	}
	pthread_mutex_unlock(&simulation->queue.queue_mutex);
	return (1);
}
