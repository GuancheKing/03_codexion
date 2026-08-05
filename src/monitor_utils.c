/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:02:53 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/05 10:48:31 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	has_any_coder_burned_out(t_simulation *simulation, long current_time)
{
	int		i;
	t_coder	*coder;
	long	last_compile_time;
	long	deadline;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		coder = &simulation->coders[i];
		if (has_completed_required_compiles(coder))
		{
			i++;
			continue ;
		}
		last_compile_time = get_last_compile_time(coder);
		deadline = last_compile_time + simulation->config.time_to_burnout;
		if (current_time >= deadline)
		{
			set_simulation_finished(simulation);
			log_state(coder, LOG_BURNOUT);
			return (true);
		}
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
