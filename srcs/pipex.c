/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:04:15 by mberne            #+#    #+#             */
/*   Updated: 2021/10/27 18:04:42 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Attend que tous les forks (pipes) aient fini leur exécution

void	wait_child_process(t_structs *s)
{
	size_t		i;

	i = 0;
	while (i < s->cmds_size)
	{
		if (is_builtin(s->cmds[i]) || (s->cmds[i].path && s->cmds[i].cmd))
		{
			if (waitpid(-1, NULL, WUNTRACED) == -1)
			{
				perror("waitpid");
				return ;
			}
		}
		i++;
	}
}

//~~ Lance la commande qu'elle reçoit

void	launch_command(t_structs *s, int in, int out, t_cmd *current)
{
	pid_t	pid;
	char	**envp;

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
		else if (is_builtin(*current) && builtins(s, *current) == -1)
			perror("built-in");
		else if (!is_builtin(*current)
			&& execve(current->path, current->cmd, envp) == -1)
			perror("execve");
		free_tab(envp, 0);
		exit(EXIT_FAILURE);
	}
	else if ((in != 0 && close(in) == -1) || (out != 1 && close(out) == -1))
		perror("close");
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
		{
			perror("pipe");
			return ;
		}
		if (s->cmds[i].fd_out == 1 && i < s->cmds_size - 1)
			s->cmds[i].fd_out = pipefd[1];
		if (is_builtin(s->cmds[i]) || get_path(s, &s->cmds[i]) != -1)
			launch_command(s, s->cmds[i].fd_in, s->cmds[i].fd_out, &s->cmds[i]);
		i++;
		if (i < s->cmds_size && s->cmds[i].fd_in == 0)
			s->cmds[i].fd_in = pipefd[0];
	}
	wait_child_process(s);
}
