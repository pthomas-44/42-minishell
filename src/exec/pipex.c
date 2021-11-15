/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dev <dev@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:04:15 by mberne            #+#    #+#             */
/*   Updated: 2021/11/15 21:24:00 by dev              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Attend que tous les forks (pipes) aient fini leur exécution

static void	wait_child_process(t_structs *s)
{
	size_t		i;
	int			status;
	bool		got_sig;

	i = 0;
	got_sig = 0;
	while (i < s->cmds_size)
	{
		if (waitpid(-1, &status, WUNTRACED) == -1)
		{
			print_error("waitpid: ", NULL, NULL, errno);
			return ;
		}
		if (WIFEXITED(status))
			g_error_number = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && !got_sig)
		{
			sig_child(WTERMSIG(status));
			got_sig = 1;
		}
		i++;
	}
}

//~~ Ferme tous les fd de la commande

static void	close_fds(t_structs *s, size_t i)
{
	if (s->cmds[i].fd_in != STDIN_FILENO && s->cmds[i].fd_in != -1
		&& close(s->cmds[i].fd_in) == -1)
		print_error("close: ", NULL, NULL, errno);
	if (s->cmds[i].fd_out != STDOUT_FILENO
		&& close(s->cmds[i].fd_out) == -1)
		print_error("close: ", NULL, NULL, errno);
	if ((i && close(s->cmds[i - 1].pipefd[STDIN_FILENO]) == -1)
		|| close(s->cmds[i].pipefd[STDOUT_FILENO]) == -1)
		print_error("close: ", NULL, NULL, errno);
	if ((i == s->cmds_size - 1 && close(s->cmds[i].pipefd[STDIN_FILENO]) == -1))
		print_error("close: ", NULL, NULL, errno);
}

//~~ Duplique les fd de la commande des les fd du terminal

static void	duplicate_fds(t_structs *s, size_t i)
{
	if (s->cmds[i].fd_in != 0 && dup2(s->cmds[i].fd_in, STDIN_FILENO) == -1)
		print_error("dup2: ", NULL, NULL, errno);
	if (s->cmds[i].fd_out != 1 && dup2(s->cmds[i].fd_out, STDOUT_FILENO) == -1)
		print_error("dup2: ", NULL, NULL, errno);
	if (s->cmds[i].fd_in == 0 && i != 0
		&& dup2(s->cmds[i - 1].pipefd[STDIN_FILENO], STDIN_FILENO) == -1)
		print_error("dup2: ", NULL, NULL, errno);
	if (s->cmds[i].fd_out == 1 && i != s->cmds_size - 1
		&& dup2(s->cmds[i].pipefd[STDOUT_FILENO], STDOUT_FILENO) == -1)
		print_error("dup2: ", NULL, NULL, errno);
	if (s->cmds[i].fd_in != 0 && close(s->cmds[i].fd_in) == -1)
		print_error("close: ", NULL, NULL, errno);
	if (s->cmds[i].fd_out != 1 && close(s->cmds[i].fd_out) == -1)
		print_error("close: ", NULL, NULL, errno);
	if (close(s->cmds[i].pipefd[STDIN_FILENO]) != 0
		&& close(s->cmds[i].pipefd[STDOUT_FILENO]) == -1)
		print_error("close: ", NULL, NULL, errno);
}

//~~ Exécution de la commande

static void	child(t_structs *s, t_cmd *current, size_t i)
{
	char	**envp;

	if (s->cmds[i].fd_in == -1)
	{
		free_all(s, 1);
		exit(g_error_number);
	}
	signal(SIGINT, NULL);
	signal(SIGQUIT, NULL);
	envp = list_to_char(s);
	duplicate_fds(s, i);
	if (is_builtin(*current) != 1 && path_error_check(current) == -1)
	{
		free(current->path);
		current->path = NULL;
	}
	else if (is_builtin(*current) == 1)
		builtins(s, current, STDOUT_FILENO);
	else if (current->path && execve(current->path, current->cmd, envp) == -1)
		print_error("execve: ", NULL, NULL, errno);
	free_array(&envp, 0);
	free_all(s, 1);
	exit(g_error_number);
}

//~~ Création d'un process par commande

void	pipex(t_structs *s)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	while (i < s->cmds_size)
	{
		if (pipe(s->cmds[i].pipefd) == -1)
			print_error("pipe: ", NULL, NULL, errno);
		if (!is_builtin(s->cmds[i]))
			get_path(s, &s->cmds[i]);
		pid = fork();
		if (pid == -1)
		{
			print_error("fork: ", NULL, NULL, errno);
			close_fds(s, i);
			break ;
		}
		else if (pid == 0)
			child(s, &s->cmds[i], i);
		else
			close_fds(s, i);
		i++;
	}
	wait_child_process(s);
}
