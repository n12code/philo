/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:39:17 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/21 17:02:10 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*cs;
	size_t			i;

	cs = (unsigned char *) s;
	i = 0;
	while (n--)
	{
		cs[i] = (unsigned char) c;
		i++;
	}
	return (cs);
}

size_t	ft_strlcpy(char *dst, const char *src, int size)
{
	size_t	len_src;
	int		i;

	len_src = 0;
	i = 0;
	while (src[len_src])
		len_src++;
	if (size <= 0)
		return (len_src);
	while (src[i] && size - 1)
	{
		dst[i] = src[i];
		i++;
		size--;
	}
	dst[i] = 0;
	return (len_src);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int		i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && n - 1
		&& (unsigned char) s1[i] == (unsigned char) s2[i])
	{
		i++;
		n--;
	}
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	try_take_fork(t_mutex *fork, int philo_id)
{
	int	success;

	success = 0;
	pthread_mutex_lock(&fork->mutex);
	if (fork->owner == -1)
	{
		fork->owner = philo_id;
		success = 1;
	}
	pthread_mutex_unlock(&fork->mutex);
	return (success);
}

void	release_fork(t_mutex *fork)
{
	pthread_mutex_lock(&fork->mutex);
	fork->owner = -1;
	pthread_mutex_unlock(&fork->mutex);
}
