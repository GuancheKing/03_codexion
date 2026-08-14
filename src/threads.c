/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:48:33 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/05 11:16:40 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(&simulation->coders[i].thread, NULL,
				coder_routine, &simulation->coders[i]) != 0)
			return (1);
		simulation->created_threads++;
		i++;
	}
	if (pthread_create(&simulation->monitor, NULL,
			monitor_routine, simulation) != 0
	)
		return (1);
	simulation->monitor_created = true;
	return (0);
}

int	join_threads(t_simulation *simulation)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (i < simulation->created_threads)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			error = 1;
		i++;
	}
	if (simulation->monitor_created == true)
	{
		if (pthread_join(simulation->monitor, NULL) != 0)
			error = 1;
	}
	return (error);
}

void	cancel_start(t_simulation *simulation)
{
	set_simulation_finished(simulation);
	pthread_mutex_lock(&simulation->start_mutex);
	simulation->start_ready = true;
	pthread_cond_broadcast(&simulation->start_condition);
	pthread_mutex_unlock(&simulation->start_mutex);
}
