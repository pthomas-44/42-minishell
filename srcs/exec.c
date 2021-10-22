/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/20 14:25:12 by mberne           ###   ########lyon.fr   */
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

void	exec_cmds(t_structs *s)
{
	size_t	i;

	if (find_cmd_paths(s) == -1)
	{
		perror("malloc");
		return ;
	}
	i = 0;
	while (i < s->cmds_size)
	{
		if (!s->cmds[i].path && !is_builtin(s->cmds[i]))
			find_executable_path(s, s->cmds[i]);
		i++;
	}
	if (s->cmds->cmd)
		pipex(s);
}
