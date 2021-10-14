/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/13 16:57:16 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_export(t_structs *s)
{
	(void)s;
}

void	create_env_variable(t_structs *s, char *current)
{
	(void)s;
	(void)current;
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
				create_env_variable(s, current.cmd[i]);
			i++;
		}
	}
	else
		print_export(s);
}
