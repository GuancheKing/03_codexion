#ifndef CODEXION_H
# define CODEXION_H

typedef enum
{
    SCHEDULER_FIFO,
    SCHEDULER_EDF
} t_scheduler;

typedef struct
{
    int         number_of_coders;
    long        time_to_burnout;
    long        time_to_compile;
    long        time_to_debug;
    long        time_to_refactor;
    int         number_of_compiles_required;
    long        dongle_cooldown;
    t_scheduler scheduler;
} t_config;

int	parser(int argc, char **argv, t_config *config);

#endif