/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:45:00 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/21 22:47:54 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef	enum
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
} t_scheduler;

typedef struct
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
} t_config;

int		parser(int argc, char **argv, t_config *config);
long	get_time_ms(void);

#endif