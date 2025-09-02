/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:00:39 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/01 16:25:53 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init(int argc, char **argv, t_data *data)
{
	if (init_data(argc, argv, data))
		return (1);
	if (init_forks(data))
		return (1);
	if (init_philos(data))
		return (1);
	return (0);
}

int	check_input(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (1);
	if (check_input_digits(argv))
		return (1);
	if (check_input_valid(argc, argv))
		return (1);
	return (0);
}

int	threads(t_data *data)
{
	if (create_threads(data))
		return (1);
	if (join_threads(data))
		return (1);
	return (0);
}

int	philosophers(int argc, char **argv)
{
	t_data	data;
	
	if (init(argc, argv, &data))
	{
		free_data(&data);
		return (1);
	}
	if (threads(&data))
	{
		free_data(&data);
		return (1);
	}
	free_data(&data);
	return (0);
}

int	main(int argc, char **argv)
{
	if (check_input(argc, argv))
		return (1);
	if (philosophers(argc, argv))
		return (1);
	return (0);
}
