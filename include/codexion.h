/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 22:45:00 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 20:12:17 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef enum e_claim_result
{
	CLAIM_ERROR,
	CLAIM_WAIT,
	CLAIM_SUCCESS
}	t_claim_result;

typedef enum e_log_state
{
	LOG_DONGLE,
	LOG_COMPILING,
	LOG_DEBUGGING,
	LOG_REFACTORING,
	LOG_BURNOUT
}	t_log_state;

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
typedef struct s_coder		t_coder;

typedef struct s_request
{
	t_coder		*coder;
	long		arrival_order;
	long		deadline;
}	t_request;

typedef struct s_request_queue
{
	t_request		*items;
	int				size;
	int				capacity;
	long			next_arrival_order;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
}	t_request_queue;

typedef struct s_dongle
{
	int				id;
	long			available_at;
	bool			in_use;
	pthread_mutex_t	dongle_mutex;
}	t_dongle;

struct s_coder
{
	int				id;
	int				completed_compiles;
	long			last_compile_time;
	pthread_t		thread;
	t_simulation	*simulation;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t	state_mutex;
};

struct s_simulation
{
	t_config		config;
	long			simulation_start;
	bool			start_ready;
	pthread_mutex_t	start_mutex;
	pthread_cond_t	start_condition;
	bool			simulation_finished;
	pthread_mutex_t	finish_mutex;
	pthread_mutex_t	log_mutex;
	int				created_threads;
	t_coder			*coders;
	t_dongle		*dongles;
	t_request_queue	queue;
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
bool	request_has_priority(t_request *a, t_request *b, t_scheduler scheduler);
int		init_request_queue(t_simulation *simulation);
void	swap_requests(t_request *a, t_request *b);
int		parent_index(int i);
int		left_child_index(int i);
int		right_child_index(int i);
int		push_request(
			t_request_queue *queue,
			t_request request,
			t_scheduler scheduler
			);
int		pop_request(
			t_request_queue *queue,
			t_request *result,
			t_scheduler scheduler
			);
int		enqueue_coder_request(t_coder *coder);
bool	try_reserve_dongles(t_coder *coder, long current_time);
void	release_dongles(t_coder *coder, long release_time);
int		wait_for_dongles(t_coder *coder);
void	cancel_dongle_reservation(t_coder *coder);
void	get_coder_dongles_wait_info(
			t_coder *coder,
			bool *has_busy_dongle,
			long *available_at
			);
int		stop_dongle_wait(t_simulation *simulation);
bool	coder_is_queue_head(t_request_queue *queue, t_coder *coder);
int		log_state(t_coder *coder, t_log_state state);
void	init_default_values(t_simulation *simulation);
int		init_resources(t_simulation *simulation);

#endif