/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/10/27 18:29:04 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Change la variable d'environnement pwd

int	set_pwd(t_structs *s)
{
	char	cwd[MAXPATHLEN];
	t_env	*pwd;

	pwd = *s->env;
	while (pwd)
	{
		if (!ft_strcmp(pwd->name, "PWD"))
			break ;
		pwd = pwd->next;
	}
	getcwd(cwd, MAXPATHLEN);
	free(pwd->value);
	pwd->value = ft_strdup(cwd);
	if (!pwd->value)
		return (-1);
	return (0);
}

//~~ Change la variable d'environnement oldpwd

int	set_oldpwd(t_structs *s)
{
	t_env	*pwd;
	t_env	*old_pwd;

	pwd = *s->env;
	while (pwd)
	{
		if (!ft_strcmp(pwd->name, "PWD"))
		{
			old_pwd = *s->env;
			while (old_pwd)
			{
				if (!ft_strcmp(old_pwd->name, "OLDPWD"))
				{
					free(old_pwd->value);
					old_pwd->value = ft_strdup(pwd->value);
					if (!old_pwd->value)
						return (-1);
				}
				old_pwd = old_pwd->next;
			}
		}
		pwd = pwd->next;
	}
	return (0);
}

//~~ Built-in cd

void	bi_cd(t_structs *s, t_cmd current)
{
	if (!current.cmd[1] || current.cmd[1][0] == '~')
		current.path = replace_by_home_path(s, current.cmd[1]);
	else
		current.path = ft_strdup(current.cmd[1]);
	if (!current.path)
	{
		errno = EXIT_FAILURE;
		return ;
	}
	if (chdir(current.path) == -1)
	{
		write(2, "potatoshell: cd: ", 17);
		write(2, current.path, ft_strlen(current.path));
		write(2, ": No such file or directory\n", 28);
		errno = EXIT_FAILURE;
		return ;
	}
	if (set_oldpwd(s) == -1 || set_pwd(s) == -1)
	{
		errno = EXIT_FAILURE;
		return ;
	}
}
