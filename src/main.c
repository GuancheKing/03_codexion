/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:31:59 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 18:24:24 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	simulation;

	if (init_simulation(argc, argv, &simulation) == 1)
		return (1);
	if (start_threads(&simulation) == 1)
	{
		cancel_start(&simulation);
		join_threads(&simulation);
		destroy_simulation(&simulation);
		return (1);
	}
	if (start_simulation(&simulation) == 1)
	{
		join_threads(&simulation);
		destroy_simulation(&simulation);
		return (1);
	}
	if (join_threads(&simulation) == 1)
	{
		destroy_simulation(&simulation);
		return (1);
	}
	destroy_simulation(&simulation);
	return (0);
}
