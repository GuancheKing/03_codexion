/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:45:00 by josjimen          #+#    #+#             */
/*   Updated: 2026/07/23 15:12:56 by josjimen         ###   ########.fr       */
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

typedef struct s_dongle
{
	int				id;
	long			available_at;
	bool			in_use;
	pthread_mutex_t	dongle_mutex;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				completed_compiles;
	long			last_compile_time;
	pthread_t		thread;
	t_simulation	*simulation;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coder;

struct s_simulation
{
	t_config		config;
	long			simulation_start;
	bool			start_ready;
	pthread_mutex_t	start_mutex;
	pthread_cond_t	start_condition;
	bool			simulation_finished;
	pthread_mutex_t	finish_mutex;
	int				created_threads;
	t_coder			*coders;
	t_dongle		*dongles;
};

int		parser(int argc, char **argv, t_config *config);
long	get_time_ms(void);
int		init_simulation(int argc, char **argv, t_simulation *simulation);
void	destroy_simulation(t_simulation *simulation);
void	*coder_routine(void *arg);
int		start_threads(t_simulation *simulation);
int		join_threads(t_simulation *simulation);
int		start_simulation(t_simulation *simulation);
void	cancel_start(t_simulation *simulation);
bool	is_simulation_finished(t_simulation *simulation);
void	set_simulation_finished(t_simulation *simulation);
int		wait_ms(t_simulation *simulation, long duration);
int		init_dongles(t_simulation *simulation);
void	assign_dongles(t_simulation *simulation);

#endif