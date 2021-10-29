/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 10:55:39 by mberne            #+#    #+#             */
/*   Updated: 2021/10/29 14:13:14 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

	i = 0;
	while (++i <= s->env_size)
	{
		export = *s->env;
		while (export)
		{
			if (i == export->index)
			{
				write(STDOUT_FILENO, "declare -x ", 11);
				write(STDOUT_FILENO, export->name, ft_strlen(export->name));
				if (ft_strlen(export->value) > 0)
				{
					write(STDOUT_FILENO, "=\"", 2);
					write(STDOUT_FILENO, export->value + 1,
						ft_strlen(export->value) - 1);
					write(STDOUT_FILENO, "\"", 1);
				}
				write(STDOUT_FILENO, "\n", 1);
			}
			export = export->next;
		}
	}
}

//~~ Crée une nouvelle variable d'environnement

static void	create_variable(t_structs *s, char *cmd, char *tmp)
{
	t_env	*export;

	export = *s->env;
	while (export)
	{
		if (!ft_strncmp(tmp, export->name, ft_strlen(export->name) + 1))
			break ;
		export = export->next;
	}
	if (export)
		env_del(s, export);
	env_new(s, cmd);
}

//~~ Vérifie si la variable qu'on veut créer est valable

static int	create_env_variable(t_structs *s, t_cmd current)
{
	size_t	i;
	char	*tmp;

	i = 1;
	while (current.cmd[i])
	{
		tmp = take_name(current.cmd[i]);
		if (!tmp)
			return (-1);
		if (is_word(tmp))
			create_variable(s, current.cmd[i], tmp);
		else
		{
			errno = 1;
			write(2, "potatoshell: export: `", 22);
			write(2, current.cmd[i], ft_strlen(current.cmd[i]));
			write(2, "': not a valid identifier\n", 26);
		}
		free(tmp);
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
			errno = EXIT_FAILURE;
			return ;
		}
	}
	else
	{
		index_list(s);
		print_export(s);
	}
}
