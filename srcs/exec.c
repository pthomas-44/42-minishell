/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/27 18:04:45 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Lance le built-in qui correspond à la commande

int	builtins(t_structs *s, t_cmd current)
{
	if (!ft_strcmp(current.cmd[0], "echo") && bi_echo(current) == -1)
		return (-1);
	else if (!ft_strcmp(current.cmd[0], "cd") && bi_cd(s, current) == -1)
		return (-1);
	else if (!ft_strcmp(current.cmd[0], "pwd") && bi_pwd() == -1)
		return (-1);
	else if (!ft_strcmp(current.cmd[0], "export")
		&& bi_export(s, current) == -1)
		return (-1);
	else if (!ft_strcmp(current.cmd[0], "unset") && bi_unset(s, current) == -1)
		return (-1);
	else if (!ft_strcmp(current.cmd[0], "env") && bi_env(s) == -1)
		return (-1);
	// else if (!ft_strcmp(current.cmd[0], "exit"))
	// 	ft_exit(s, "exit", ft_atoi(current.cmd[1]));
	return (0);
}

//~~ Renvoie 1 si la commande est un built-in

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

//~~ Exécution de la ligne de commande

void	exec(t_structs *s)
{
	if (s->cmds_size == 1 && is_builtin(s->cmds[0]))
	{
		if (builtins(s, s->cmds[0]) == -1)
			perror("built-in");
	}
	else
		pipex(s);
}
