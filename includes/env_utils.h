/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 20:23:23 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/19 22:25:12 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include <stddef.h>
# include <stdbool.h>

typedef struct s_shellstate	t_shellstate;

size_t	env_key_len(const char *env_entry);
int		env_get_entry_index(const char *entry, const char **envp);
bool	env_entry_is_valid(const char *entry);
bool	env_name_is_valid(const char *name);
int		ft_setenv(const char *name, const char *value, t_shellstate *state);
int		ft_setenv_entry(const char *entry, t_shellstate *state);
void	remove_env_var(char *var_key, t_shellstate *s);
void	update_shell_level(t_shellstate *state);
int		get_shell_level(char *const envp[]);
long	str_to_long(const char *str, int *overflow);
int		is_numeric(const char *str);

#endif