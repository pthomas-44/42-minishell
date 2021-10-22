/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 13:00:45 by mberne            #+#    #+#             */
/*   Updated: 2021/10/22 11:36:02 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	launch_builtin(t_structs *s, int in, int out, t_cmd current)
{
	if (in != 0 && dup2(in, current.fd_in) == -1)
	{
		perror("dup2");
		return ;
	}
	if (out != 1 && dup2(out, current.fd_out) == -1)
	{
		perror("dup2");
		return ;
	}
	if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
	{
		perror("close");
		return ;
	}
	builtins(s, current);
}

void	launch_command(t_structs *s, int in, int out, t_cmd current)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_exit(s, "fork", EXIT_FAILURE);
	else if (pid == 0)
	{
		if (in != 0 && dup2(in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return ;
		}
		if (out != 1 && dup2(out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return ;
		}
		if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
		{
			perror("close");
			return ;
		}
		if (execve(current.path, current.cmd, NULL) == -1)
		{
			write(2, "minishell: ", 11);
			write(2, current.cmd[0], ft_strlen(current.cmd[0]));
			write(2, ": command not found\n", 20);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
		{
			perror("close");
			return ;
		}
	}
}

void	pipex(t_structs *s)
{
	int			pipefd[2];
	int			in;
	size_t		i;

	i = 0;
	in = s->cmds[i].fd_in;
	while (i < s->cmds_size - 1)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (is_builtin(s->cmds[i]))
			launch_builtin(s, in, pipefd[1], s->cmds[i]);
		else
			launch_command(s, in, pipefd[1], s->cmds[i]);
		in = pipefd[0];
		i++;
	}
	if (is_builtin(s->cmds[i]))
		launch_builtin(s, in, s->cmds[i].fd_out, s->cmds[i]);
	else
		launch_command(s, in, s->cmds[i].fd_out, s->cmds[i]);
	i = 0;
	while (i < s->cmds_size && !is_builtin(s->cmds[i]))
	{
		if (waitpid(-1, NULL, WUNTRACED) == -1)
		{
			perror("waitpid");
			return ;
		}
		i++;
	}
}

// int pipe_fd[2]

// while (i < s->cmds_size - 1)
// {
// 	if (cmd[i].fd_in > 0)
// 		dup2(cmd[i].fd_in, STDIN);
// 	else if (pipe_fd[1] > 0)
// 		dup2(pipe_fd[0], STDIN);
// 	pipe_fd[1] = -1;
// 	if (cmd[i].fd_out > 0)
// 		dup2(cmd[i].fd_out, STDOUT);
// 	else
// 	{
// 		pipe(pipe_fd[2]);
// 		dup2(pipe_fd[1], STDOUT);
// 	}
// 	launch_command();
// 	i++;
// }