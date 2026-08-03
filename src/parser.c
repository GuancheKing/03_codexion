/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josjimen <josjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 12:47:17 by josjimen          #+#    #+#             */
/*   Updated: 2026/08/03 13:09:12 by josjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>
#include <limits.h>

static int	valid_nmbr(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_custom_atol(const char *str, long *result)
{
	long			res;
	int				digit;
	unsigned int	i;

	res = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = (str[i] - '0');
		if (res > (LONG_MAX - digit) / 10)
			return (1);
		res = res * 10 + digit;
		i++;
	}
	*result = res;
	return (0);
}

static int	parse_nmbr(char **argv, t_config *config)
{
	int		i;
	long	values[7];

	i = 0;
	while (i < 7)
	{
		if (valid_nmbr(argv[i + 1]) == 0)
			return (1);
		if (ft_custom_atol(argv[i + 1], &values[i]) == 1)
			return (1);
		if (values[i] <= 0 && i != 6)
			return (1);
		i++;
	}
	if (values[0] > INT_MAX || values[5] > INT_MAX)
		return (1);
	config->number_of_coders = (int)values[0];
	config->time_to_burnout = values[1];
	config->time_to_compile = values[2];
	config->time_to_debug = values[3];
	config->time_to_refactor = values[4];
	config->number_of_compiles_required = (int)values[5];
	config->dongle_cooldown = values[6];
	return (0);
}

static int	parse_schdlr(char *argv8, t_config *config)
{
	if (strcmp(argv8, "fifo") == 0)
		config->scheduler = SCHEDULER_FIFO;
	else if (strcmp(argv8, "edf") == 0)
		config->scheduler = SCHEDULER_EDF;
	else
		return (1);
	return (0);
}

int	parser(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (1);
	if (parse_nmbr(argv, config) == 1)
		return (1);
	if (parse_schdlr(argv[8], config) == 1)
		return (1);
	return (0);
}
