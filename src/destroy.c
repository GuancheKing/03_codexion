/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 09:25:28 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 10:12:58 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	destroy_simulation(t_simulation *simulation)
{
	pthread_mutex_destroy(&simulation->finish_mutex);
	free(simulation->coders);
	simulation->coders = NULL;
}
