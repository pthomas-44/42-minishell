/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/18 17:27:05 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtins_or_not(t_structs *s, t_cmd current)
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
	else
	{
		if (execve(current.path, current.cmd, NULL) == -1)
		{
			write(2, "minishell: ", 11);
			write(2, current.cmd[0], ft_strlen(current.cmd[0]));
			write(2, ": command not found", 19);
		}
	}
	return (0);
}

void	exec_cmds(t_structs *s)
{
	if (find_cmd_paths(s) == -1)
	{
		perror("malloc");
		return ;
	}
	pipex(s);
}
