/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/11/11 11:33:13 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Remplace le '~' par le HOME

int	replace_by_home_path(t_structs *s, char *path, char **new)
{
	t_env	*elem;

	elem = find_env_var(s, "HOME");
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

//~~ Set les variables d'environnement PWD et OLDPWD

static int	set_pwd(t_structs *s, char *cwd)
{
	t_env	*pwd;
	t_env	*old_pwd;

	pwd = find_env_var(s, "PWD");
	old_pwd = find_env_var(s, "OLDPWD");
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
		print_error("cd: ", current->path, NULL, errno);
		g_numberr = EXIT_FAILURE;
	}
	else if (!current->path || set_pwd(s, cwd) == -1)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_numberr = EXIT_FAILURE;
	}
}
