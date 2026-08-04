/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 22:59:53 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/04 08:28:44 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	cancel_dongle_and_finish_sim(t_coder *coder)
{
	cancel_dongle_reservation(coder);
	set_simulation_finished(coder->simulation);
	return (1);
}

static int	start_compilation(t_coder *coder)
{
	long			current_time;
	t_simulation	*simulation;

	simulation = coder->simulation;
	if (enqueue_coder_request(coder) == 1)
	{
		set_simulation_finished(simulation);
		return (1);
	}
	if (wait_for_dongles(coder) == 1)
		return (1);
	current_time = get_time_ms();
	if (current_time == -1)
		return (cancel_dongle_and_finish_sim(coder));
	set_last_compile_time(coder, current_time);
	if (log_state(coder, LOG_DONGLE) == 1)
		return (cancel_dongle_and_finish_sim(coder));
	if (log_state(coder, LOG_DONGLE) == 1)
		return (cancel_dongle_and_finish_sim(coder));
	if (log_state(coder, LOG_COMPILING) == 1)
		return (cancel_dongle_and_finish_sim(coder));
	return (0);
}

static int	finish_compilation(t_coder *coder)
{
	long			release_time;
	t_simulation	*simulation;

	simulation = coder->simulation;
	if (wait_ms(simulation, simulation->config.time_to_compile) == 1)
	{
		release_time = get_time_ms();
		if (release_time == -1)
			return (cancel_dongle_and_finish_sim(coder));
		release_dongles(coder, release_time);
		return (1);
	}
	release_time = get_time_ms();
	if (release_time == -1)
		return (cancel_dongle_and_finish_sim(coder));
	increment_completed_compiles(coder);
	release_dongles(coder, release_time);
	return (0);
}

static int	run_debug_and_refactor(t_coder *coder)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	if (has_completed_required_compiles(coder))
		return (0);
	if (log_state(coder, LOG_DEBUGGING) == 1)
	{
		set_simulation_finished(simulation);
		return (1);
	}
	if (wait_ms(simulation, simulation->config.time_to_debug) == 1)
		return (1);
	if (log_state(coder, LOG_REFACTORING) == 1)
	{
		set_simulation_finished(simulation);
		return (1);
	}
	if (wait_ms(simulation, simulation->config.time_to_refactor) == 1)
		return (1);
	return (0);
}

int	run_coder_cycle(t_coder *coder)
{
	if (start_compilation(coder) == 1)
		return (1);
	if (finish_compilation(coder) == 1)
		return (1);
	if (run_debug_and_refactor(coder) == 1)
		return (1);
	return (0);
}
