/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:31:49 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/21 22:46:11 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>
#include <stddef.h>

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
