/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 10:39:29 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_pwd(t_structs *s, t_env *pwd)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	pwd->value = NULL;
	pwd->value = ft_strdup(cwd);
	if (!pwd->value)
		ft_exit(s, "malloc", EXIT_FAILURE);
}

t_env	*set_oldpwd(t_structs *s)
{
	t_env	*pwd;
	t_env	*old_pwd;

	pwd = *s->env;
	while (pwd)
	{
		if (!ft_strcmp(pwd->name, "PWD"))
		{
			old_pwd = pwd;
			while (old_pwd)
			{
				if (!ft_strcmp(old_pwd->name, "OLDPWD"))
				{
					old_pwd->value = NULL;
					old_pwd->value = ft_strdup(pwd->value);
					if (!old_pwd->value)
						ft_exit(s, "malloc", EXIT_FAILURE);
					break ;
				}
				old_pwd = old_pwd->next;
			}
			break ;
		}
		pwd = pwd->next;
	}
	return (pwd);
}

void	ft_cd(t_structs *s, t_cmd current)
{
	t_env	*pwd;

	if (chdir(current.cmd[1]) == -1)
		ft_exit(s, "chdir", EXIT_FAILURE);
	pwd = set_oldpwd(s);
	set_pwd(s, pwd);
}
