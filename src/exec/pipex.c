/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:04:15 by mberne            #+#    #+#             */
/*   Updated: 2021/11/11 13:47:52 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Attend que tous les forks (pipes) aient fini leur exécution

static void	wait_child_process(t_structs *s)
{
	size_t		i;
	int			status;

	i = 0;
	while (i < s->cmds_size)
	{
		if (waitpid(-1, &status, WUNTRACED) == -1)
		{
			print_error("waitpid: ", NULL, NULL, errno);
			return ;
		}
		if (WIFEXITED(status))
			g_numberr = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			g_numberr = 130;
		}
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
			g_numberr = 131;
		}
		i++;
	}
}

// ~~ Recupere le chemin d'une commande

static void	get_path(t_structs *s, t_cmd *current)
{
	char	**paths;

	if (!current->cmd)
		return ;
	paths = get_env_paths(s);
	if (paths && !ft_strchr(current->cmd[0], '/')
		&& find_path_in_sys(current, paths) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	else if ((!paths || ft_strchr(current->cmd[0], '/'))
		&& !current->path && find_exe_path(s, current) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	free_tab(&paths, 0);
}

//~~ Ferme tous les fd du programme

void	close_pipe(t_structs *s)
{
	size_t	i;

	i = 0;
	while (i < s->cmds_size)
	{
		if (s->cmds[i].fd_in != STDIN_FILENO
			&& s->cmds[i].fd_in != -1
			&& close(s->cmds[i].fd_in) == -1)
			print_error("close: ", NULL, NULL, errno);
		if (s->cmds[i].fd_out != STDOUT_FILENO
			&& close(s->cmds[i].fd_out) == -1)
			print_error("close: ", NULL, NULL, errno);
		if (close(s->cmds[i].pipefd[STDIN_FILENO]) == -1
			|| close(s->cmds[i].pipefd[STDOUT_FILENO]) == -1)
			print_error("close: ", NULL, NULL, errno);
		i++;
	}
}

//~~ Exécution de la commande

static void	child(t_structs *s, t_cmd *current, size_t i)
{
	char	**envp;

	envp = list_to_char(s);
	if ((current->fd_in != 0 && dup2(current->fd_in, STDIN_FILENO) == -1)
		|| (current->fd_out != 1 && dup2(current->fd_out, STDOUT_FILENO) == -1))
		print_error("dup2: ", NULL, NULL, errno);
	else if ((current->fd_in == 0 && i != 0
			&& dup2(s->cmds[i - 1].pipefd[STDIN_FILENO], STDIN_FILENO) == -1)
		|| (current->fd_out == 1 && i != s->cmds_size - 1
			&& dup2(current->pipefd[STDOUT_FILENO], STDOUT_FILENO) == -1))
		print_error("dup2: ", NULL, NULL, errno);
	close_pipe(s);
	if (is_builtin(*current) != 1 && path_error_check(current) == -1)
	{
		free(current->path);
		current->path = NULL;
	}
	else if (is_builtin(*current) == 1)
		builtins(s, current, STDOUT_FILENO);
	else if (current->path && execve(current->path, current->cmd, envp) == -1)
		print_error("execve: ", NULL, NULL, errno);
	free_tab(&envp, 0);
}

//~~ Création d'un process par commande

void	pipex(t_structs *s)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	while (i < s->cmds_size)
	{
		if (!is_builtin(s->cmds[i]))
			get_path(s, &s->cmds[i]);
		pid = fork();
		if (pid == -1)
			print_error("fork: ", NULL, NULL, errno);
		else if (pid == 0)
		{
			signal(SIGINT, NULL);
			signal(SIGQUIT, NULL);
			if (s->cmds[i].fd_in != -1)
				child(s, &s->cmds[i], i);
			free_all(s, 1);
			exit(g_numberr);
		}
		i++;
	}
	close_pipe(s);
	wait_child_process(s);
}
