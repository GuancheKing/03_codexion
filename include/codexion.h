/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:45:00 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/22 09:49:27 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_simulation	t_simulation;

typedef struct s_coder
{
	int				id;
	int				completed_compiles;
	long			last_compile_time;
	t_simulation	*simulation;
}	t_coder;

struct s_simulation
{
	t_config		config;
	long			simulation_start;
	bool			simulation_finished;
	pthread_mutex_t	finish_mutex;
	t_coder			*coders;
};

int		parser(int argc, char **argv, t_config *config);
long	get_time_ms(void);
int		init_simulation(int argc, char **argv, t_simulation *simulation);
void	destroy_simulation(t_simulation *simulation);

#endif