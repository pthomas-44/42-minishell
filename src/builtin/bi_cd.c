/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/11/05 16:13:50 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// ~~ Remplace le '~' par le HOME

int	replace_by_home_path(t_structs *s, char *cmd, char **new)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "HOME"))
			break ;
		elem = elem->next;
	}
	if (!elem || (cmd && cmd[0] != '~'))
	{
		*(new) = ft_strdup(cmd);
		return (-1);
	}
	if (cmd)
		*(new) = ft_strjoin_f0(elem->value + 1, cmd + 1);
	else
		*(new) = ft_strdup(elem->value + 1);
	return (0);
}

//~~ Set les variables d'environnement PWD et OLDPWD

static int	set_pwd(t_structs *s, char *cwd)
{
	t_env	*elem;
	t_env	*pwd;
	t_env	*old_pwd;

	elem = *s->env;
	pwd = NULL;
	old_pwd = NULL;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "PWD"))
			pwd = elem;
		else if (!ft_strcmp(elem->name, "OLDPWD"))
			old_pwd = elem;
		elem = elem->next;
	}
	if (old_pwd)
		old_pwd->value = ft_strjoin_f2("=", ft_strdup(cwd));
	getcwd(cwd, MAXPATHLEN);
	if (pwd)
		pwd->value = ft_strjoin_f2("=", ft_strdup(cwd));
	return (0);
}

//~~ Built-in cd

void	bi_cd(t_structs *s, t_cmd current)
{
	char	cwd[MAXPATHLEN];

	g_numberr = EXIT_SUCCESS;
	getcwd(cwd, MAXPATHLEN);
	if (!current.cmd[1] || current.cmd[1][0] == '~')
	{
		if (replace_by_home_path(s, current.cmd[1], &current.path) == -1)
		{
			print_error("cd: ", NULL, "HOME not set\n", EXIT_FAILURE);
			return ;
		}
	}
	else
		current.path = ft_strdup(current.cmd[1]);
	if (current.path && chdir(current.path) == -1)
	{
		print_error("cd: ", NULL, NULL, ENOENT);
		g_numberr = EXIT_FAILURE;
	}
	else if (!current.path || set_pwd(s, cwd) == -1)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_numberr = EXIT_FAILURE;
	}
}
