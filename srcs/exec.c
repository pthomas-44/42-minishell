/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/20 20:55:00 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtins(t_structs *s, t_cmd current)
{
	if (!ft_strcmp(current.cmd[0], "echo"))
		ft_echo(current);
	else if (!ft_strcmp(current.cmd[0], "cd"))
		ft_cd(s, current);
	else if (!ft_strcmp(current.cmd[0], "pwd"))
		ft_pwd(current);
	else if (!ft_strcmp(current.cmd[0], "export"))
		ft_export(s, current);
	else if (!ft_strcmp(current.cmd[0], "unset"))
		ft_unset(s, current);
	else if (!ft_strcmp(current.cmd[0], "env"))
		ft_env(s, current);
	else if (!ft_strcmp(current.cmd[0], "exit"))
		ft_exit(s, "", errno);
}

int	is_builtin(t_cmd current)
{
	if (!ft_strcmp(current.cmd[0], "echo"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "cd"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "pwd"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "export"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "unset"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "env"))
		return (1);
	else if (!ft_strcmp(current.cmd[0], "exit"))
		return (1);
	return (0);
}
