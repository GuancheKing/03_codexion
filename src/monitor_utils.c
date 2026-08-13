/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:02:53 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/13 15:58:08 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	get_coder_state_snapshot(
	t_coder *coder,
	int *completed_compiles,
	long *last_compile_time
)
{
	pthread_mutex_lock(&coder->state_mutex);
	*completed_compiles = coder->completed_compiles;
	*last_compile_time = coder->last_compile_time;
	pthread_mutex_unlock(&coder->state_mutex);
}

static bool	check_coder_burnout(
	t_simulation *simulation, t_coder *coder, long current_time
)
{
	int		completed_compiles;
	long	last_compile_time;
	long	deadline;

	get_coder_state_snapshot(
		coder,
		&completed_compiles,
		&last_compile_time
		);
	if (
		completed_compiles
		>= simulation->config.number_of_compiles_required
	)
		return (false);
	deadline = last_compile_time + simulation->config.time_to_burnout;
	if (current_time >= deadline)
	{
		set_simulation_finished(simulation);
		log_state(coder, LOG_BURNOUT);
		return (true);
	}
	return (false);
}

bool	has_any_coder_burned_out(t_simulation *simulation, long current_time)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		coder = &simulation->coders[i];
		if (check_coder_burnout(simulation, coder, current_time))
			return (true);
		i++;
	}
	return (false);
}

bool	have_all_coders_completed(t_simulation *simulation)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		coder = &simulation->coders[i];
		if (has_completed_required_compiles(coder) == false)
			return (false);
		i++;
	}
	return (true);
}
