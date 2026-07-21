#include "codexion.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	t_config    config;

	if (parser(argc, argv, &config) == 1)
		return (1);
	// DEBUGGING
	// printf("number_of_coders: %d\n", config.number_of_coders);
	// printf("time_to_burnout: %ld\n", config.time_to_burnout);
	// printf("time_to_compile: %ld\n", config.time_to_compile);
	// printf("time_to_debug: %ld\n", config.time_to_debug);
	// printf("time_to_refactor: %ld\n", config.time_to_refactor);
	// printf("number_of_compiles_required: %d\n",
	// 	config.number_of_compiles_required);
	// printf("dongle_cooldown: %ld\n", config.dongle_cooldown);
	// if (config.scheduler == SCHEDULER_FIFO)
	// 	printf("fifo\n");
	// else
	// 	printf("edf\n");
	return (0);
}