/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:25:08 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/09 19:01:33 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	overflow_checker(char c, long *nb, int *overflow)
{
	long	prev;

	prev = *nb;
	*nb = *nb * 10 + (c - 48);
	if (*nb < prev || *nb > 2147483647)
		*overflow = 1;
}

long	ft_atol(char *str, int *overflow)
{
	int		i;
	long	nb;

	i = 0;
	nb = 0;
	if (!str[i])
	{
		*overflow = 1;
		return (0);
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		overflow_checker(str[i], &nb, overflow);
		if (*overflow != 0)
			return (0);
		i++;
	}
	if (str[i] != '\0')
	{
		*overflow = 1;
		return (0);
	}
	return (nb);
}

int	ft_atoi(char *str)
{
	int		overflow;
	long	result;

	overflow = 0;
	result = ft_atol(str, &overflow);
	if (overflow)
		return (-1);
	return ((int)result);
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
