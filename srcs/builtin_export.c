/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/14 14:01:17 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	index_list(t_structs *s)
{
	(void)s;
}

void	print_export(t_structs *s)
{
	size_t	i;
	t_env	*current;

	index_list(s);
	i = 0;
	while (i < s->env_size)
	{
		current = *s->env;
		while (current)
		{
			if (i = current->index)
			{
				// write "declare -x "
				// write name
				// write "=""
				// write value + 1
				// write ""\n"
				break ;
			}
			current = current->next;
		}
		i++;
	}
}

void	create_env_variable(t_structs *s, char *current)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		ft_exit(s, "malloc", EXIT_FAILURE);
	// new->name = ;
	// new->value = ;
	new->index = s->env_size + 1;
	s->env_size++;
	new->next = NULL;
	if (!(*s->env))
		*s->env = new;
	else
	{
		while ((*s->env)->next)
			*s->env = (*s->env)->next;
		*s->env = new;
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
				create_env_variable(s, current.cmd[i]);
			i++;
		}
	}
	else
		print_export(s);
}
