/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/08 17:34:04 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(t_structs *s, int option)
{
	int	i;

	if (!option)
		i = 1;
	else
		i = 2;
	while (s->cmds[2].cmd[i])
	{
		write(s->fdout, s->cmds[2].cmd[i], ft_strlen(s->cmds[2].cmd[i]));
		i++;
	}
	if (!option)
		write(s->fdout, "\n", 1);
}

void	ft_cd(t_structs *s)
{
	(void)s;
}

void	ft_pwd(t_structs *s)
{
	int		i;
	int		j;
	char	*pwd;

	i = 0;
	while (s->env[i])
	{
		if (!ft_strncmp(s->env[i], "PWD=", 4))
		{
			j = i;
			while (s->env[j])
			{
				if (!ft_strncmp(s->env[j], "OLDPWD=", 4))
				{
					s->env[j] = NULL;
					s->env[j] = ft_strjoin_f0("OLDPWD=", s->env[i] + 4);
					if (!s->env[j])
						ft_exit(s, "malloc", EXIT_FAILURE);
					break ;
				}
				j++;
			}
			break ;
		}
		i++;
	}
	pwd = malloc(sizeof(char) * 100); // 100 au pif...
	if (!pwd)
		ft_exit(s, "malloc", EXIT_FAILURE);
	pwd = getcwd(pwd, 100);
	if (!pwd)
		ft_exit(s, "getcwd", EXIT_FAILURE);
	s->env[i] = ft_strdup(pwd);
	if (!s->env[i])
	{
		free(pwd);
		ft_exit(s, "malloc", EXIT_FAILURE);
	}
	write(s->fdout, pwd, ft_strlen(pwd));
	write(s->fdout, "\n", 1);
	free(pwd);
}

void	ft_export(t_structs *s)
{
	(void)s;
}

void	ft_unset(t_structs *s)
{
	(void)s;
}

void	ft_env(t_structs *s)
{
	(void)s;
}
