/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:16:01 by mberne            #+#    #+#             */
/*   Updated: 2021/11/02 17:08:24 by mberne           ###   ########lyon.fr   */
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

//~~ Récupère l'exit status dans une string s'il n'y en a bien qu'un

int	ft_atoi_exit(char *arg, int *exit_status)
{
	int	i;

	*(exit_status) = 0;
	i = 0;
	while (arg[i] == ' ')
		i++;
	while (arg[i] >= '0' && arg[i] <= '9')
	{
		*(exit_status) = *(exit_status) * 10 + (arg[i] - '0');
		i++;
	}
	while (arg[i] == ' ')
		i++;
	if (arg[i])
		return (-1);
	return (0);
}
