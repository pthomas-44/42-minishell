/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:16:01 by mberne            #+#    #+#             */
/*   Updated: 2021/11/02 14:26:53 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ~~ Trouve le '~' par le HOME

char	*replace_by_home_path(t_structs *s, char *cmd)
{
	t_env	*elem;
	char	*new;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "HOME"))
			break ;
		elem = elem->next;
	}
	if (cmd && (cmd[0] != '~' || ft_strcmp(elem->name, "HOME")))
		return (ft_strdup(cmd));
	if (cmd)
		new = ft_strjoin_f0(elem->value + 1, cmd + 1);
	else
		new = ft_strdup(elem->value + 1);
	return (new);
}

//~~ Vérifie l'option -n pour echo

int	check_option_n(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

//~~ Vérifie qu'une variable a le bon format selon POSIX

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

//~~ Récupère le nom de la future variable d'environnement dans la commande

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
