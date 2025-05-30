/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:38:21 by nbodin            #+#    #+#             */
/*   Updated: 2025/05/28 16:36:05 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	
	if (argc < 5 || argc > 6)
		return (1);
	if (check_input(argc, argv))
		return (1);
	if (init(&data, argc, argv))
		return (1);
	//one philo case
	if (launch_threads(&data))
	//launch threads
	//free
}