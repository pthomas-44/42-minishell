/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:08 by mberne            #+#    #+#             */
/*   Updated: 2021/10/22 17:38:17 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtins(t_structs *s, t_cmd current)
{
	if (!ft_strcmp(current.cmd[0], "echo"))
		ft_echo(s, current);
	else if (!ft_strcmp(current.cmd[0], "cd"))
		ft_cd(s, current);
	else if (!ft_strcmp(current.cmd[0], "pwd"))
		ft_pwd(s, current);
	else if (!ft_strcmp(current.cmd[0], "export"))
		ft_export(s, current);
	else if (!ft_strcmp(current.cmd[0], "unset"))
		ft_unset(s, current);
	else if (!ft_strcmp(current.cmd[0], "env"))
		ft_env(s, current);
	else if (!ft_strcmp(current.cmd[0], "exit"))
		ft_exit(s, "", errno);
}

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

void	wait_child_process(t_structs *s)
{
	size_t		i;

	i = 0;
	while (i < s->cmds_size && !is_builtin(s->cmds[i]))
	{
		if (s->cmds[i].path && s->cmds[i].cmd
			&& waitpid(-1, NULL, WUNTRACED) == -1)
		{
			perror("waitpid");
			return ;
		}
		i++;
	}
}

void	launch_command(t_structs *s, int in, int out, t_cmd *current)
{
	pid_t	pid;
	char	**envp;

	if (get_path(s, current) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid == 0)
	{
		envp = list_to_char(s);
		if (in != 0 && dup2(in, STDIN_FILENO) == -1)
			perror("dup2");
		else if (out != 1 && dup2(out, STDOUT_FILENO) == -1)
			perror("dup2");
		else if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
			perror("close");
		else if (execve(current->path, current->cmd, envp) == -1)
			perror("execve");
		free_tab(envp, 0);
	}
	else
		if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
			perror("close");
}

void	pipex(t_structs *s)
{
	int			pipefd[2];
	size_t		i;

	i = 0;
	while (i < s->cmds_size)
	{
		if (i < s->cmds_size - 1 && pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (s->cmds[i].fd_out == 1 && i < s->cmds_size - 1)
			s->cmds[i].fd_out = pipefd[1];
		if (is_builtin(s->cmds[i]))
			builtins(s, s->cmds[i]);
		else
			launch_command(s, s->cmds[i].fd_in, s->cmds[i].fd_out, &s->cmds[i]);
		i++;
		if (s->cmds[i].fd_in == 0 && i)
			s->cmds[i].fd_in = pipefd[0];
	}
	wait_child_process(s);
}
