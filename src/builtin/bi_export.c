/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/11/15 13:10:48 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	print_export(t_structs *s, int fd)
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
				ft_putstr_fd("declare -x ", fd);
				ft_putstr_fd(export->name, fd);
				if (export->value)
				{
					ft_putstr_fd("=\"", fd);
					ft_putstr_fd(export->value + 1, fd);
					ft_putchar_fd('"', fd);
				}
				ft_putchar_fd('\n', fd);
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
	return (env_new(s, cmd));
}

//~~ Vérifie si la variable qu'on veut créer est valable

static void	create_env_variable(t_structs *s, t_cmd *current)
{
	size_t	i;
	char	*name;

	i = 1;
	while (current->cmd[i])
	{
		if (!ft_strchr(current->cmd[i], '='))
			name = ft_strdup(current->cmd[i]);
		else
			name = ft_substr(current->cmd[i], 0,
					ft_strchr(current->cmd[i], '=') - current->cmd[i]);
		if (!name)
			print_error("malloc: ", NULL, NULL, ENOMEM);
		else if (!current->cmd[i][0] || current->cmd[i][0] == '='
			|| !is_word(name))
			print_error("export: ", current->cmd[i],
				"not a valid identifier\n", EXIT_FAILURE);
		else if (create_variable(s, current->cmd[i], name) == -1)
			print_error("malloc: ", NULL, NULL, ENOMEM);
		free(name);
		i++;
	}
}

//~~ Built-in export

void	bi_export(t_structs *s, t_cmd *current, int fd)
{
	g_error_number = EXIT_SUCCESS;
	if (current->cmd[1])
		create_env_variable(s, current);
	else
	{
		index_list(s);
		print_export(s, fd);
	}
}
