/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:48:33 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 14:35:49 by josjimen         ###   ########.fr       */
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
	return (0);
}

int	join_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->created_threads)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
