/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 23:27:15 by apyykone          #+#    #+#             */
/*   Updated: 2023/10/30 11:37:12 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*algo(char *answ, int i, int n, int count)
{
	int	zeros;
	int	j;

	zeros = 1;
	if (n < 0)
	{
		answ[i] = '-';
		i++;
		n = -n;
	}
	while (i < count)
	{
		zeros = 1;
		j = i;
		while (j < count - 1)
		{
			zeros *= 10;
			j++;
		}
		answ[i] = (n / zeros) + '0';
		n %= zeros;
		i++;
	}
	return (answ);
}

static void	checknums(int *n, int *n2, int *count)
{
	if (*n < 0)
	{
		*count += 1;
		*n2 = *n * -1;
	}
	else
		*n2 = *n;
	while (*n2 >= 10)
	{
		*count += 1;
		*n2 /= 10;
	}
}

char	*ft_itoa(int n)
{
	int		i;
	int		n2;
	char	*answ;
	int		count;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	count = 1;
	i = 0;
	checknums(&n, &n2, &count);
	answ = (char *)malloc(sizeof(char) * (count + 1));
	if (!answ)
		return (NULL);
	answ[count] = '\0';
	return (algo(answ, i, n, count));
}
