/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/11/09 17:50:23 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Lance le built-in qui correspond à la commande

void	builtins(t_structs *s, t_cmd *current, int fd)
{
	if (!ft_strcmp(current->cmd[0], "echo"))
		bi_echo(current, fd);
	else if (!ft_strcmp(current->cmd[0], "cd"))
		bi_cd(s, current);
	else if (!ft_strcmp(current->cmd[0], "pwd"))
		bi_pwd(fd);
	else if (!ft_strcmp(current->cmd[0], "export"))
		bi_export(s, current, fd);
	else if (!ft_strcmp(current->cmd[0], "unset"))
		bi_unset(s, current);
	else if (!ft_strcmp(current->cmd[0], "env"))
		bi_env(s, fd);
	else if (!ft_strcmp(current->cmd[0], "exit"))
		bi_exit(s, current);
}

//~~ Renvoie 1 si la commande est un built-in

int	is_builtin(t_cmd current)
{
	if (!current.cmd || !current.cmd[0])
		return (-1);
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
	if (s->cmds_size == 1 && is_builtin(s->cmds[0]) == 1)
		builtins(s, &s->cmds[0], s->cmds[0].fd_out);
	else
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->term[OLD]) == -1)
			print_error("termios: ", NULL, NULL, errno);
		pipex(s);
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->term[NEW]) == -1)
			print_error("termios: ", NULL, NULL, errno);
		signal(SIGINT, &sig_int);
	}
}
