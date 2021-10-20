/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/20 12:58:15 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	index_list(t_structs *s)
{
	t_env	*elem;
	t_env	*next;

	elem = *s->env;
	while (elem)
	{
		elem->index = 1;
		next = *s->env;
		while (next)
		{
			if (ft_strcmp(elem->name, next->name) > 0)
				elem->index++;
			next = next->next;
		}
		elem = elem->next;
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
				write(current.fd_out, "\n", 1);
			}
			export = export->next;
		}
	}
}

int	is_word(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*take_name(char *arg)
{
	size_t	i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	if (!name)
		return (NULL);
	return (name);
}

int	create_env_variable(t_structs *s, t_cmd current)
{
	size_t	i;
	t_env	*export;
	char	*tmp;

	i = 1;
	while (current.cmd[i])
	{
		export = *s->env;
		tmp = take_name(current.cmd[i]);
		if (!tmp)
			return (-1);
		if (is_word(tmp))
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
		else
		{
			write(2, "minishell: export: '", 20);
			write(2, current.cmd[i], ft_strlen(current.cmd[i]));
			write(2, "': not a valid identifier\n", 26);
		}
		i++;
	}
	return (0);
}

void	ft_export(t_structs *s, t_cmd current)
{
	if (current.cmd[1])
	{
		if (create_env_variable(s, current) == -1)
		{
			errno = EXIT_FAILURE;
			return ;
		}
	}
	else
	{
		index_list(s);
		print_export(s, current);
	}
}
