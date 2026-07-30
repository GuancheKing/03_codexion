/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_heap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 05:47:29 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/30 14:54:14 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heapify_up(
	t_request_queue *queue,
	int index,
	t_scheduler scheduler
)
{
	int	parent;

	while (index > 0)
	{
		parent = parent_index(index);
		if (request_has_priority(
				&queue->items[index],
				&queue->items[parent],
				scheduler
			) == false)
			break ;
		swap_requests(&queue->items[index], &queue->items[parent]);
		index = parent;
	}
}

static void	heapify_down(
	t_request_queue *queue,
	int index,
	t_scheduler scheduler
)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = left_child_index(index);
		right = right_child_index(index);
		best = index;
		if (left < queue->size && request_has_priority(
				&queue->items[left], &queue->items[best], scheduler
			))
			best = left;
		if (right < queue->size && request_has_priority(
				&queue->items[right], &queue->items[best], scheduler
			))
			best = right;
		if (best == index)
			break ;
		swap_requests(&queue->items[index], &queue->items[best]);
		index = best;
	}
}

int	push_request(
	t_request_queue *queue,
	t_request request,
	t_scheduler scheduler
)
{
	int	index;

	if (queue->size >= queue->capacity)
		return (1);
	index = queue->size;
	queue->items[index] = request;
	queue->size++;
	heapify_up(queue, index, scheduler);
	return (0);
}

int	pop_request(
	t_request_queue *queue,
	t_request *result,
	t_scheduler scheduler
)
{
	if (queue->size == 0)
		return (1);
	*result = queue->items[0];
	queue->size--;
	if (queue->size > 0)
	{
		queue->items[0] = queue->items[queue->size];
		heapify_down(queue, 0, scheduler);
	}
	return (0);
}
