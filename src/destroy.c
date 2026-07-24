/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 09:25:28 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/23 15:29:27 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	destroy_simulation(t_simulation *simulation)
{
	int	i;

	i = 0;
	if (simulation->dongles != NULL)
	{
		while (i < simulation->config.number_of_coders)
		{
			pthread_mutex_destroy(&simulation->dongles[i].dongle_mutex);
			i++;
		}
	}
	free(simulation->dongles);
	simulation->dongles = NULL;
	pthread_mutex_destroy(&simulation->finish_mutex);
	pthread_cond_destroy(&simulation->start_condition);
	pthread_mutex_destroy(&simulation->start_mutex);
	free(simulation->coders);
	simulation->coders = NULL;
}
