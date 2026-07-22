/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 06:55:36 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 07:15:59 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(int argc, char **argv, t_simulation *simulation)
{
	if (parser(argc, argv, &simulation->config) == 1)
		return (1);
	simulation->simulation_start = get_time_ms();
	if (simulation->simulation_start == -1)
		return (1);
	simulation->simulation_finished = false;
	return (0);
}
