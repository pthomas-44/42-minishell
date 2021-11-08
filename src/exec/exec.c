/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/11/08 13:55:12 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ Lance le built-in qui correspond à la commande

void	builtins(t_structs *s, t_cmd current)
{
	if (!ft_strcmp(current.cmd[0], "echo"))
		bi_echo(current);
	else if (!ft_strcmp(current.cmd[0], "cd"))
		bi_cd(s, current);
	else if (!ft_strcmp(current.cmd[0], "pwd"))
		bi_pwd();
	else if (!ft_strcmp(current.cmd[0], "export"))
		bi_export(s, current);
	else if (!ft_strcmp(current.cmd[0], "unset"))
		bi_unset(s, current);
	else if (!ft_strcmp(current.cmd[0], "env"))
		bi_env(s);
	else if (!ft_strcmp(current.cmd[0], "exit"))
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
		builtins(s, s->cmds[0]);
	else
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->term[OLD]) == -1)
			print_error("termios: ", NULL, NULL, errno);
		signal(SIGINT, &child_sig_int);
		signal(SIGQUIT, &child_sig_quit);
		pipex(s);
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->term[NEW]) == -1)
			print_error("termios: ", NULL, NULL, errno);
		signal(SIGINT, &sig_int);
	}
}
