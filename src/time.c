/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:31:49 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/13 17:45:25 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>
#include <stddef.h>
#include <unistd.h>

long	get_time_ms(void)
{
	struct timeval	time;
	long			result_t;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	result_t = (
			time.tv_sec * 1000 + time.tv_usec / 1000
			);
	return (result_t);
}

int	wait_ms(t_simulation *simulation, long duration)
{
	long	start;
	long	current;

	start = get_time_ms();
	if (start == -1)
	{
		set_simulation_finished(simulation);
		return (1);
	}
	while (is_simulation_finished(simulation) == false)
	{
		current = get_time_ms();
		if (current == -1)
		{
			set_simulation_finished(simulation);
			return (1);
		}
		if ((current - start) >= duration)
			return (0);
		usleep(1000);
	}
	return (1);
}
