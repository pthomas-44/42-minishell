/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/10/18 15:55:26 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	go_home(t_structs *s, t_cmd current)
{
	t_env	*elem;
	char	*tmp;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "HOME"))
			break ;
		elem = elem->next;
	}
	if (current.cmd[1])
	{
		tmp = ft_strdup(current.cmd[1] + 1);
		free(current.cmd[1]);
		if (!tmp)
			return (-1);
		current.cmd[1] = ft_strjoin_f2(elem->value + 1, tmp);
		if (!current.cmd[1])
			return (-1);
	}
	else
	{
		current.cmd[1] = ft_strdup(elem->value + 1);
		if (!current.cmd[1])
			return (-1);
	}
	return (0);
}

int	set_pwd(t_env *pwd)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	free(pwd->value);
	pwd->value = ft_strdup(cwd);
	if (!pwd->value)
		return (-1);
	return (0);
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
					free(old_pwd->value);
					old_pwd->value = ft_strdup(pwd->value);
					if (!old_pwd->value)
						return (NULL);
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

	if (!current.cmd[1] || current.cmd[1][0] == '~')
	{
		if (go_home(s, current) == -1)
		{
			errno = EXIT_FAILURE;
			return ;
		}
	}
	if (chdir(current.cmd[1]) == -1)
	{
		write(2, "minishell: cd: ", 15);
		write(2, current.cmd[1], ft_strlen(current.cmd[1]));
		write(2, ": No such file or directory", 27);
		errno = EXIT_FAILURE;
		return ;
	}
	pwd = set_oldpwd(s);
	if (!pwd)
	{
		errno = EXIT_FAILURE;
		return ;
	}
	if (set_pwd(pwd) == -1)
	{
		errno = EXIT_FAILURE;
		return ;
	}
}
