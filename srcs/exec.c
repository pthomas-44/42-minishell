/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 19:29:55 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtins_or_not(t_structs *s, t_cmd current)	// mettre la fonctions de type "int"
{
	if (!ft_strcmp(current.cmd[0], "echo"))	// je te propose qu'on fasse un tableau de pointeur sur fontcion tout les deux
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
		ft_exit(s, "", EXIT_SUCCESS);	// remplacer par "ft_exit(s, "", errno);"
	else
		if (execve(current.path, current.cmd, NULL) == -1)
			ft_exit(s, "execve", EXIT_FAILURE);	// remplacer par "write(2, "minishell: ", 11);	write(2, current.cmd[0], ft_strlen(current.cmd[0]));	write "2, ": command not found", 19";	return (-1);
		// pipex(s); plutôt
}	// rajouter "return(0)"

void	exec_cmds(t_structs *s)
{
	find_cmd_paths(s);	// remplacer par "if (find_cmd_paths(s) == -1){	perror("malloc");	return ;}"
	if (s->cmds_size == 1)
		builtins_or_not(s, s->cmds[0]);
	else
		pipex(s);
}
