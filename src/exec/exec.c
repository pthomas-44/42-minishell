/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/11/14 18:58:13 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Gestion d'erreur de la recuperation du chemin

int	path_error_check(t_cmd *current)
{
	DIR	*dir;
	int	fd;

	if (!current->path && !current->cmd)
		return (-1);
	dir = opendir(current->path);
	fd = open(current->path, O_RDONLY);
	if (!current->path)
	{
		print_error(NULL, current->cmd[0], "command not found\n", 127);
		if (dir)
			closedir(dir);
	}
	else if (dir)
	{
		print_error(NULL, current->path, NULL, EISDIR);
		closedir(dir);
	}
	else if (fd == -1 || close(fd) == -1)
		print_error(NULL, current->cmd[0], NULL, ENOENT);
	else if (access(current->path, X_OK) == -1)
		print_error(NULL, current->cmd[0], NULL, EACCES);
	else
		return (0);
	return (-1);
}

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
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->old_term) == -1)
			print_error("termios: ", NULL, NULL, errno);
		signal(SIGINT, SIG_IGN);
		pipex(s);
		if (tcsetattr(STDIN_FILENO, TCSANOW, &s->new_term) == -1)
			print_error("termios: ", NULL, NULL, errno);
		signal(SIGINT, &sig_int);
	}
}
