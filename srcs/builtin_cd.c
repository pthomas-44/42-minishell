/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/10/18 09:57:01 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_pwd(t_structs *s, t_env *pwd)	// mettre la fonctions de type "int"
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	pwd->value = NULL;	// remplacer par "free(pwd->value);"
	pwd->value = ft_strdup(cwd);
	if (!pwd->value)
		ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par return (-1);
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
					old_pwd->value = NULL;	// remplacer par "free(old_pwd->value);"
					old_pwd->value = ft_strdup(pwd->value);
					if (!old_pwd->value)
						ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par return (NULL);
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

	if (current.cmd[1])
	{
		if (chdir(current.cmd[1]) == -1)
			ft_exit(s, "chdir", EXIT_FAILURE);	// remplacer par "write(2, "minishell: cd: ", 11);	write(2, current.cmd[1], ft_strlen(current.cmd[0]));	write "2, ": No such file or directory", 28";	return ;""
		pwd = set_oldpwd(s);	// rajouter "if (!pwd){	return ;}"
		set_pwd(s, pwd);	// remplacer par "if (set_pwd(s, pwd) == -1){	return ;}"
	}
	// else cd tout seul revient dans HOME
}
