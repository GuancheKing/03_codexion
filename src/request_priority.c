/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_priority.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:03:34 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/24 10:18:43 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	request_has_priority(t_request *a, t_request *b, t_scheduler scheduler)
{
	if (scheduler == SCHEDULER_FIFO)
		return (a->arrival_order < b->arrival_order);
	if (scheduler == SCHEDULER_EDF)
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
		return (a->arrival_order < b->arrival_order);
	}
	return (false);
}
