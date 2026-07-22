/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 09:25:28 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 15:13:24 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	destroy_simulation(t_simulation *simulation)
{
	pthread_mutex_destroy(&simulation->finish_mutex);
	pthread_cond_destroy(&simulation->start_condition);
	pthread_mutex_destroy(&simulation->start_mutex);
	free(simulation->coders);
	simulation->coders = NULL;
}
