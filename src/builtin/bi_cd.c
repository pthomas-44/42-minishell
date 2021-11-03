/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/11/03 04:55:29 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// ~~ Remplace le '~' par le HOME

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

	errno = EXIT_SUCCESS;
	getcwd(cwd, MAXPATHLEN);
	if (!current.cmd[1] || current.cmd[1][0] == '~')
		current.path = replace_by_home_path(s, current.cmd[1]);
	else
		current.path = ft_strdup(current.cmd[1]);
	if (!current.path)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		errno = EXIT_FAILURE;
	}
	else if (chdir(current.path) == -1)
	{
		print_error("cd: ", NULL, NULL, ENOENT);
		errno = EXIT_FAILURE;
	}
	else if (set_pwd(s, cwd) == -1)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		errno = EXIT_FAILURE;
	}
}
