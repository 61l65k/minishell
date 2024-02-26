/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:32:17 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/26 21:13:02 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir(char *path)
{
	char	*home_path;

	if (path == NULL)
	{
		home_path = getenv("HOME");
		if (home_path == NULL)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return ;
		}
		else if (chdir(home_path) == -1)
		{
			ft_fprintf(2, "cd: %s: %s\n", home_path, strerror(errno));
		}
	}
	else if (chdir(path) == -1)
	{
		ft_fprintf(2, "cd: %s: %s\n", path, strerror(errno));
	}
}
