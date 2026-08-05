/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:39:20 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/05 11:16:41 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static void	wait_for_simulation_start(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->start_mutex);
	while (simulation->start_ready == false)
	{
		pthread_cond_wait(
			&simulation->start_condition,
			&simulation->start_mutex);
	}
	pthread_mutex_unlock(&simulation->start_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_simulation_start(coder->simulation);
	if (is_simulation_finished(coder->simulation) == true)
		return (NULL);
	while (is_simulation_finished(coder->simulation) == false
		&& has_completed_required_compiles(coder) == false)
	{
		if (run_coder_cycle(coder) == 1)
			break ;
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*simulation;
	long			current_time;

	simulation = (t_simulation *)arg;
	wait_for_simulation_start(simulation);
	if (is_simulation_finished(simulation) == true)
		return (NULL);
	while (is_simulation_finished(simulation) == false)
	{
		current_time = get_time_ms();
		if (current_time == -1)
		{
			set_simulation_finished(simulation);
			return (NULL);
		}
		if (has_any_coder_burned_out(simulation, current_time))
			return (NULL);
		if (have_all_coders_completed(simulation))
		{
			set_simulation_finished(simulation);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
