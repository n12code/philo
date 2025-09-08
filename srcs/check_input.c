/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:25:08 by nbodin            #+#    #+#             */
/*   Updated: 2025/09/08 17:30:26 by nbodin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long ft_atol(char *str, int *overflow)
{
    int     i;
    long    nb;
    long    prev;
    
    i = 0;
    nb = 0;
    *overflow = 0;
    
    // Skip leading whitespace
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
           str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
        i++;
    
    // Check for empty string after whitespace
    if (!str[i])
    {
        *overflow = 1;
        return (0);
    }
    
    while ('0' <= str[i] && str[i] <= '9')
    {
        prev = nb;
        nb = nb * 10 + (str[i] - '0');
        
        // Check for overflow: if nb < prev, we wrapped around
        if (nb < prev || nb > 2147483647) // INT_MAX
        {
            *overflow = 1;
            return (0);
        }
        i++;
    }
    
    // Check if there are non-digit characters after the number
    if (str[i] != '\0')
    {
        *overflow = 1;
        return (0);
    }
    
    return (nb);
}

int ft_atoi(char *str)
{
    int overflow;
    long result;
    
    result = ft_atol(str, &overflow);
    if (overflow)
        return (-1); // Return -1 to indicate error
    return ((int)result);
}

int check_input_digits(char **argv)
{
    size_t  i;
    size_t  j;
    
    i = 1;
    while (argv[i])
    {
        j = 0;
        // Skip leading whitespace
        while (argv[i][j] == ' ' || argv[i][j] == '\t' || argv[i][j] == '\n' || 
               argv[i][j] == '\r' || argv[i][j] == '\f' || argv[i][j] == '\v')
            j++;
        
        // Check if empty after whitespace
        if (!argv[i][j])
            return (1);
        
        // Check for leading zeros (except single "0")
        if (argv[i][j] == '0' && argv[i][j + 1] >= '0' && argv[i][j + 1] <= '9')
            return (1);
        
        // Check all characters are digits
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

int check_input_valid(int argc, char **argv)
{
    int     overflow;
    long    value;
    size_t  i;

    // Check number of philosophers
    value = ft_atol(argv[1], &overflow);
    if (overflow || value < 1 || value > 200)
        return (1);

    // Check time values (must be >= 60ms)
    i = 2;
    while (i < 5)
    {
        value = ft_atol(argv[i], &overflow);
        if (overflow || value < 60)
            return (1);
        i++;
    }
    
    // Check number of meals if provided
    if (argc == 6)
    {
        value = ft_atol(argv[5], &overflow);
        if (overflow || value <= 0)
            return (1);
    }
    
    return (0);
}