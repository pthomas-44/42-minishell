/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 11:19:07 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtins_or_not(t_structs *s, t_cmd current)
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
		ft_exit(s, "", EXIT_SUCCESS);
	else
		if (execve(current.path, current.cmd, NULL) == -1)
			ft_exit(s, "execve", EXIT_FAILURE);
}

void	exec_cmds(t_structs *s)
{
	find_cmd_paths(s);
	if (s->cmds_size == 1)
		builtins_or_not(s, s->cmds[0]);
	else
		pipex(s);
}
