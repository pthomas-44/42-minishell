/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/11/05 15:48:50 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ Index les variables d'environnement selon l'ordre ascii

static void	index_list(t_structs *s)
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

//~~ Print les variables d'environnement triées selon l'ordre ascii

static void	print_export(t_structs *s)
{
	size_t	i;
	t_env	*export;

	i = 1;
	while (i <= s->env_size)
	{
		export = *s->env;
		while (export)
		{
			if (i == export->index)
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(export->name, STDOUT_FILENO);
				if (ft_strlen(export->value) > 0)
				{
					ft_putstr_fd("=\"", STDOUT_FILENO);
					ft_putstr_fd(export->value + 1, STDOUT_FILENO);
					ft_putchar_fd('"', STDOUT_FILENO);
				}
				ft_putchar_fd('\n', STDOUT_FILENO);
			}
			export = export->next;
		}
		i++;
	}
}

//~~ Crée une nouvelle variable d'environnement

static int	create_variable(t_structs *s, char *cmd, char *name)
{
	t_env	*export;

	export = *s->env;
	while (export && ft_strcmp(name, export->name))
		export = export->next;
	if (export)
		env_del(s, export);
	if (env_new(s, cmd) == -1)
		return (-1);
	return (0);
}

//~~ Vérifie si la variable qu'on veut créer est valable

static int	create_env_variable(t_structs *s, t_cmd current)
{
	size_t	i;
	char	*name;

	i = 1;
	while (current.cmd[i])
	{
		if (!ft_strchr(current.cmd[i], '='))
			name = ft_strdup(current.cmd[i]);
		else
			name = ft_substr(current.cmd[i], 0,
					ft_strchr(current.cmd[i], '=') - current.cmd[i]);
		if (!name)
			return (-1);
		if (current.cmd[i][0] != '=' && is_word(name))
		{
			if (create_variable(s, current.cmd[i], name) == -1)
				return (-1);
		}
		else
			print_error("export: ", current.cmd[i],
				"not a valid identifier\n", EXIT_FAILURE);
		free(name);
		i++;
	}
	return (0);
}

//~~ Built-in export

void	bi_export(t_structs *s, t_cmd current)
{
	if (current.cmd[1])
	{
		if (create_env_variable(s, current) == -1)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			errno = EXIT_FAILURE;
			return ;
		}
	}
	else
	{
		index_list(s);
		print_export(s);
	}
	errno = EXIT_SUCCESS;
}
