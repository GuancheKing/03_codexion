/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 22:33:13 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 22:59:09 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_last_compile_time(t_coder *coder, long compile_time)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_time = compile_time;
	pthread_mutex_unlock(&coder->state_mutex);
}

long	get_last_compile_time(t_coder *coder)
{
	long	result;

	pthread_mutex_lock(&coder->state_mutex);
	result = coder->last_compile_time;
	pthread_mutex_unlock(&coder->state_mutex);
	return (result);
}

void	increment_completed_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->completed_compiles++;
	pthread_mutex_unlock(&coder->state_mutex);
}

int	get_completed_compiles(t_coder *coder)
{
	int	result;

	pthread_mutex_lock(&coder->state_mutex);
	result = coder->completed_compiles;
	pthread_mutex_unlock(&coder->state_mutex);
	return (result);
}

bool	has_completed_required_compiles(t_coder *coder)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	return (
		get_completed_compiles(coder)
		>= simulation->config.number_of_compiles_required
	);
}
