/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:25:08 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/01 16:42:19 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		nb *= 10;
		nb += str[i] - 48;
		i++;
	}
	return (nb);
}

int	check_input_digits(char **argv)
{
	size_t	i;
	size_t	j;
	
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || '9' < argv[i][j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	check_input_valid(int argc, char **argv)
{
	size_t	i;

	i = 2;
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (1);
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200)
		return (1);
	while (i < 5)
	{
		if (ft_atoi(argv[i]) < 60)
			return (1);
		i++;
	}
	return (0);
}