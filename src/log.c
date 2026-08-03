/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 16:20:58 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 17:21:22 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static const char	*get_log_message(t_log_state state)
{
	if (state == LOG_DONGLE)
		return ("has taken a dongle");
	else if (state == LOG_COMPILING)
		return ("is compiling");
	else if (state == LOG_DEBUGGING)
		return ("is debugging");
	else if (state == LOG_REFACTORING)
		return ("is refactoring");
	else if (state == LOG_BURNOUT)
		return ("burned out");
	return (NULL);
}

static int	unlock_log_mutex(t_simulation *simulation, int result)
{
	pthread_mutex_unlock(&simulation->log_mutex);
	return (result);
}

int	log_state(t_coder *coder, t_log_state state)
{
	t_simulation	*simulation;
	long			current_time;
	long			timestamp;
	const char		*message;

	simulation = coder->simulation;
	message = get_log_message(state);
	if (message == NULL)
		return (1);
	pthread_mutex_lock(&simulation->log_mutex);
	if (
		(is_simulation_finished(simulation) == true)
		&& (state != LOG_BURNOUT))
		return (unlock_log_mutex(simulation, 0));
	current_time = get_time_ms();
	if (current_time == -1)
		return (unlock_log_mutex(simulation, 1));
	timestamp = current_time - simulation->simulation_start;
	if (printf("%ld %d %s\n", timestamp, coder->id, message) == -1)
		return (unlock_log_mutex(simulation, 1));
	return (unlock_log_mutex(simulation, 0));
}
