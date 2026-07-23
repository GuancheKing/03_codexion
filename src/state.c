/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 09:54:34 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/23 10:25:28 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_simulation_finished(t_simulation *simulation)
{
	bool	finished;

	pthread_mutex_lock(&simulation->finish_mutex);
	finished = simulation->simulation_finished;
	pthread_mutex_unlock(&simulation->finish_mutex);
	return (finished);
}

void	set_simulation_finished(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->finish_mutex);
	simulation->simulation_finished = true;
	pthread_mutex_unlock(&simulation->finish_mutex);
}
