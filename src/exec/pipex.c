/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:04:15 by mberne            #+#    #+#             */
/*   Updated: 2021/11/08 13:28:04 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ Attend que tous les forks (pipes) aient fini leur exécution

static void	wait_child_process(t_structs *s)
{
	size_t		i;
	int			status;

	i = 0;
	while (i < s->cmds_size)
	{
		if (is_builtin(s->cmds[i]) || (s->cmds[i].path && s->cmds[i].cmd))
		{
			if (waitpid(-1, &status, WUNTRACED) == -1)
			{
				print_error("waitpid: ", NULL, NULL, errno);
				return ;
			}
			if (WIFEXITED(status))
				g_numberr = WEXITSTATUS(status);
		}
		i++;
	}
}

//~~ Lance la commande qu'elle reçoit

static void	launch_command(t_structs *s, int in, int out, t_cmd *current)
{
	pid_t	pid;
	char	**envp;

	pid = fork();
	if (pid == -1)
		print_error("fork: ", NULL, NULL, errno);
	else if (pid == 0)
	{
		envp = list_to_char(s);
		if ((in != 0 && dup2(in, STDIN_FILENO) == -1)
			|| (out != 1 && dup2(out, STDOUT_FILENO) == -1))
			print_error("dup2: ", NULL, NULL, errno);
		else if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
			print_error("close: ", NULL, NULL, errno);
		else if (!is_builtin(*current)
			&& execve(current->path, current->cmd, envp) == -1)
			print_error("execve: ", NULL, NULL, errno);
		else if (is_builtin(*current))
			builtins(s, *current);
		free_tab(&envp, 0);
		free_all(s, 1);
		exit(errno);
	}
	else if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
		print_error("close: ", NULL, NULL, errno);
}

// ~~ Recupere le chemin d'une commande

static int	get_path(t_structs *s, t_cmd *current)
{
	char	**paths;

	if (!current->cmd)
		return (-1);
	paths = get_env_paths(s);
	if (paths && !ft_strchr(current->cmd[0], '/')
		&& find_path_in_sys(current, paths) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	else if ((!paths || ft_strchr(current->cmd[0], '/'))
		&& !current->path && find_exe_path(s, current) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	else if (path_error_check(current) == -1)
	{
		free(current->path);
		current->path = NULL;
	}
	else
	{
		free_tab(&paths, 0);
		return (0);
	}
	free_tab(&paths, 0);
	return (-1);
}

//~~ Exécute plusieurs commandes

void	pipex(t_structs *s)
{
	int			pipefd[2];
	size_t		i;

	i = 0;
	while (i < s->cmds_size)
	{
		if (i < s->cmds_size - 1 && pipe(pipefd) == -1)
			return ;
		if (s->cmds[i].fd_out == STDOUT_FILENO && i < s->cmds_size - 1)
			s->cmds[i].fd_out = pipefd[STDOUT_FILENO];
		if (is_builtin(s->cmds[i]) || get_path(s, &s->cmds[i]) != -1)
			launch_command(s, s->cmds[i].fd_in, s->cmds[i].fd_out, &s->cmds[i]);
		else if ((s->cmds[i].fd_in != 0 && close(s->cmds[i].fd_in) == -1)
			|| (s->cmds[i].fd_out != 1 && close(s->cmds[i].fd_out) == -1))
			print_error("close: ", NULL, NULL, errno);
		if (i < s->cmds_size - 1 && s->cmds[i].fd_out != 1
			&& s->cmds[i].fd_out != pipefd[STDOUT_FILENO]
			&& close(pipefd[STDOUT_FILENO]) == -1)
			print_error("close: ", NULL, NULL, errno);
		if (++i < s->cmds_size && s->cmds[i].fd_in == STDIN_FILENO)
			s->cmds[i].fd_in = pipefd[STDIN_FILENO];
		else if (i < s->cmds_size && close(pipefd[STDIN_FILENO]) == -1)
			print_error("close: ", NULL, NULL, errno);
	}
	wait_child_process(s);
}

// int	open_pipe(t_structs *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < s->cmds_size)
// 	{
// 		if (pipe(s->cmds[i].pipefd) == -1)
// 			return (-1);
// 	}
// 	return (0);
// }

// void	pipex(t_structs *s)
// {
// 	if (open_pipe(s) == -1)
// 	{
// 		print_error("pipe: ", NULL, NULL, errno);
// 		return ;
// 	}
// }
