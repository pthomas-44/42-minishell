/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 13:00:45 by mberne            #+#    #+#             */
/*   Updated: 2021/10/11 11:23:24 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	launch_command(t_structs *s, int in, int out, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_exit(s, "fork", EXIT_FAILURE);
	else if (pid == 0)
	{
		if (in != 0 && dup2(in, STDIN_FILENO) == -1)
			ft_exit(s, "dup2", EXIT_FAILURE);
		if (out != 1 && dup2(out, STDOUT_FILENO) == -1)
			ft_exit(s, "dup2", EXIT_FAILURE);
		if (close(in) == -1 || close(out) == -1)
			ft_exit(s, "close", EXIT_FAILURE);
		if (execve(s->cmds[i].path, s->cmds[i].cmd, NULL) == -1)
			ft_exit(s, "execve", EXIT_FAILURE);
	}
	if (close(in) == -1 || close(out) == -1)
		ft_exit(s, "close", EXIT_FAILURE);
	return (pid);
}

void	pipex(t_structs *s)
{
	int			pipefd[2];
	int			in;
	int			i;

	in = s->fdin;
	i = 0;
	while (i < s->num_cmd - 1)
	{
		if (pipe(pipefd) == -1)
			ft_exit(s, "pipe", EXIT_FAILURE);
		s->cmds[i].pid = launch_command(s, in, pipefd[1], i);
		in = pipefd[0];
		i++;
	}
	s->cmds[i].pid = launch_command(s, in, s->fdout, i);
	i = 0;
	while (i < s->num_cmd)
	{
		if (waitpid(s->cmds[i].pid, NULL, WUNTRACED) == -1)
			ft_exit(s, "waitpid", EXIT_FAILURE);
		i++;
	}
}

// int pipe_fd[2]

// while (i < s->num_cmd - 1)
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