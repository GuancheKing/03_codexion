/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_heap utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 05:47:29 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/30 10:15:57 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_requests(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	parent_index(int i)
{
	return ((i -1) / 2);
}

int	left_child_index(int i)
{
	return (2 * i + 1);
}

int	right_child_index(int i)
{
	return (2 * i + 2);
}
