/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:39:20 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 18:33:01 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_simulation_finished(t_simulation *simulation)
{
	bool	finished;

	pthread_mutex_lock(&simulation->finish_mutex);
	finished = simulation->simulation_finished;
	pthread_mutex_unlock(&simulation->finish_mutex);
	return (finished);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->simulation->start_mutex);
	while (coder->simulation->start_ready == false)
	{
		pthread_cond_wait(
			&coder->simulation->start_condition,
			&coder->simulation->start_mutex);
	}
	pthread_mutex_unlock(&coder->simulation->start_mutex);
	if (is_simulation_finished(coder->simulation) == true)
		return (NULL);
	/* Future coder work loop */
	return (NULL);
}
