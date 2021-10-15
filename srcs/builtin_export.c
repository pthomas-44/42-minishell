/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 19:36:13 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	index_list(t_structs *s)
{
	t_env	*list;	// remplacer par "t_env	*elem;"
	t_env	*next;

	list = *s->env;
	while (list)
	{
		list->index = 1;
		next = *s->env;
		while (next)
		{
			if (ft_strcmp(list->name, next->name) > 0)
				list->index++;
			next = next->next;
		}
		list = list->next;
	}
}

void	print_export(t_structs *s, t_cmd current)
{
	size_t	i;
	t_env	*export;

	i = 0;
	while (++i <= s->env_size)
	{
		export = *s->env;
		while (export)
		{
			if (i == export->index)
			{
				write(current.fd_out, "declare -x ", 11);
				write(current.fd_out, export->name, ft_strlen(export->name));
				if (ft_strlen(export->value) > 0)
				{
					write(current.fd_out, "=\"", 2);
					write(current.fd_out, export->value + 1,
						ft_strlen(export->value) - 1);
					write(current.fd_out, "\"", 1);
				}
				write(current.fd_out, "\n", 2);	// remplacer par "write(current.fd_out, "\n", 1);"
			}
			export = export->next;
		}
	}
}

char	*take_name(t_structs *s, char *arg)
{
	size_t	i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	if (!name)
		ft_exit(s, "malloc", EXIT_FAILURE); // remplacer par "return (NULL)"
	return (name);
}

void	create_env_variable(t_structs *s, t_cmd current)	// mettre la fonctions de type "int"
{
	size_t	i;
	t_env	*export;
	char	*tmp;

	i = 1;
	while (current.cmd[i])
	{
		export = *s->env;
		tmp = take_name(s, current.cmd[i]);	// rajouter "if (!tmp){	return (-1);}"
		if (ft_str_isalpha(tmp))
		{
			while (export)
			{
				if (!ft_strncmp(tmp, export->name, ft_strlen(export->name) + 1))
					break ;
				export = export->next;
			}
			free(tmp);
			if (export)
				env_del(s, export);
			env_new(s, current.cmd[i]);
		}
		i++;
	}
}	// rajouter "return (0);

void	ft_export(t_structs *s, t_cmd current)
{
	if (current.cmd[1])	// remplacer par "if (current.cmd[1] && create_env_variable(s, current) == -1){ return ;}""
		create_env_variable(s, current);
	else
	{
		index_list(s);
		print_export(s, current);
	}
}
