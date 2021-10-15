/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 13:01:09 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(t_cmd current)
{
	size_t	i;
	int		option;

	i = 1;
	option = 0;
	if (!ft_strncmp(current.cmd[i], "-n", 2))
	{
		i = 2;
		option = 1;
	}
	// while (!ft_strncmp(current.cmd[i], "-n", 2))
	// 	i++;
	while (current.cmd[i])
	{
		write(current.fd_out, current.cmd[i], ft_strlen(current.cmd[i]));
		i++;
		if (current.cmd[i])
			write(current.fd_out, " ", 1);
	}
	if (!option)
		write(current.fd_out, "\n", 1);
}

void	ft_pwd(t_cmd current)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	write(current.fd_out, cwd, ft_strlen(cwd));
	write(current.fd_out, "\n", 1);
}

void	ft_env(t_structs *s, t_cmd current)
{
	t_env	*env;

	env = *s->env;
	while (env)
	{
		if (env->value)
		{
			write(current.fd_out, env->name, ft_strlen(env->name));
			write(current.fd_out, env->value, ft_strlen(env->value));
			write(current.fd_out, "\n", 1);
		}
		env = env->next;
	}
}
