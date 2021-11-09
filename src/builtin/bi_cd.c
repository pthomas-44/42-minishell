/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/11/09 17:56:18 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Remplace le '~' par le HOME

int	replace_by_home_path(t_structs *s, char *path, char **new)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "HOME"))
			break ;
		elem = elem->next;
	}
	if (!elem || (path && path[0] != '~'))
	{
		*(new) = ft_strdup(path);
		return (-1);
	}
	else if (path)
		*(new) = ft_strjoin_f0(elem->value + 1, path + 1);
	else
		*(new) = ft_strdup(elem->value + 1);
	return (0);
}

//~~ Trouve les variables d'environnement PWD et OLDPWD

static void	find_pwd_env_var(t_structs *s,
	t_env *pwd, t_env *old_pwd)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "PWD"))
			pwd = elem;
		else if (!ft_strcmp(elem->name, "OLDPWD"))
			old_pwd = elem;
		elem = elem->next;
	}
}

//~~ Set les variables d'environnement PWD et OLDPWD

static int	set_pwd(t_structs *s, char *cwd)
{
	t_env	*pwd;
	t_env	*old_pwd;

	pwd = NULL;
	old_pwd = NULL;
	find_pwd_env_var(s, pwd, old_pwd);
	if (old_pwd)
	{
		free(old_pwd->value);
		old_pwd->value = ft_strjoin_f2("=", ft_strdup(cwd));
	}
	getcwd(cwd, MAXPATHLEN);
	if (pwd)
	{
		free(pwd->value);
		pwd->value = ft_strjoin_f2("=", ft_strdup(cwd));
	}
	if ((pwd && !pwd->value) || (old_pwd && !old_pwd->value))
		print_error("malloc: ", NULL, NULL, ENOMEM);
	return (0);
}

//~~ Built-in cd

void	bi_cd(t_structs *s, t_cmd *current)
{
	char	cwd[MAXPATHLEN];

	g_numberr = EXIT_SUCCESS;
	getcwd(cwd, MAXPATHLEN);
	if (!current->cmd[1] || current->cmd[1][0] == '~')
	{
		if (replace_by_home_path(s, current->cmd[1], &current->path) == -1)
		{
			print_error("cd: ", NULL, "HOME not set\n", EXIT_FAILURE);
			return ;
		}
	}
	else
		current->path = ft_strdup(current->cmd[1]);
	if (current->path && chdir(current->path) == -1)
	{
		print_error("cd: ", NULL, NULL, ENOENT);
		g_numberr = EXIT_FAILURE;
	}
	else if (!current->path || set_pwd(s, cwd) == -1)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_numberr = EXIT_FAILURE;
	}
}
