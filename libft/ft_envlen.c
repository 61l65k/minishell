/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:40:48 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 17:40:49 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Returns the length of the env variable.
 */
size_t	ft_envlen(char *env)
{
	size_t	len;

	len = 0;
	while (ft_isenv_var(env[len]))
		len++;
	return (len);
}
