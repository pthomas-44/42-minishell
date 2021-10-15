/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 10:56:43 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	index_list(t_structs *s)
{
	t_env	*env;
	t_env	*next;

	env = *s->env;
	while (env)
	{
		env->index = 1;
		next = *s->env;
		while (next)
		{
			if (ft_strcmp(env->name, next->name) < 0)
				env->index++;
			next = next->next;
		}
		env = env->next;
	}
}

void	print_export(t_structs *s, t_cmd current)
{
	size_t	i;
	t_env	*env;

	index_list(s);
	i = -1;
	while (++i < s->env_size)
	{
		env = *s->env;
		while (env)
		{
			if (i == env->index)
			{
				write(current.fd_out, "declare -x ", 11);
				write(current.fd_out, env->name, ft_strlen(env->name));
				if (env->value)
				{
					write(current.fd_out, "=\"", 2);
					write(current.fd_out, env->value + 1, ft_strlen(env->value) - 1);
					write(current.fd_out, "\"\n", 2);
				}
				break ;
			}
			env = env->next;
		}
	}
}

void	update_env_variable(char *current, t_env *env)
{
	size_t	i;

	i = 0;
	while (ft_isalpha(current[i]))
		i++;
	if (current[i] == '=')
	{
		env->value = NULL;
		env->value = ft_strdup(current + i + 1);
	}
}

void	ft_export(t_structs *s, t_cmd current)
{
	t_env	*env;
	size_t	i;

	if (current.cmd[1])
	{
		i = 0;
		while (current.cmd[i])
		{		
			env = *s->env;
			while (env)
			{
				if (!ft_strcmp(env->name, current.cmd[i]))
					break ;
				env = env->next;
			}
			if (env)
				update_env_variable(current.cmd[i], env);
			else
				env_new(s, current.cmd[i]);
			i++;
		}
	}
	else
		print_export(s, current);
}
