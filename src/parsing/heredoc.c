/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 11:38:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/08 11:44:09 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ La boucle de lecture du heredoc

static char	*heredoc_loop(char *stop)
{
	char	*line;
	char	*content;

	line = NULL;
	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, stop))
			break ;
		if (content)
			content = ft_strjoin_f1(content, "\n");
		content = ft_strjoin_f3(content, line);
		if (!content)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			break ;
		}
	}
	return (content);
}

//~~ Ecrit le contenu du heredoc dans le pipe

static void	heredoc(t_cmd *current, char *stop, int pipe_fd[2])
{
	char	*content;

	content = heredoc_loop(stop);
	if (content)
		ft_putstr_fd(content, pipe_fd[STDOUT_FILENO]);
	free(content);
	if (close(pipe_fd[STDOUT_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return ;
	}
	current->fd_in = pipe_fd[STDIN_FILENO];
}

//~~ Cree un process enfant pour faire fonctionner le heredoc

static void	heredoc_process_init(t_structs *s,
		t_cmd *current, char *stop, int pipe_fd[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork: ", NULL, NULL, errno);
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, &heredoc_sig_int);
		heredoc(current, stop, pipe_fd);
		free_all(s, 1);
		exit(g_numberr);
	}
	else if (waitpid(-1, &status, WUNTRACED) == -1)
	{
		print_error("waitpid: ", NULL, NULL, errno);
		return ;
	}
	if (WIFEXITED(status))
		g_numberr = WEXITSTATUS(status);
}

//~~ La gestion du heredoc

int	heredoc_handler(t_structs *s, t_cmd *current, char *stop)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		print_error("pipe: ", NULL, NULL, errno);
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	heredoc_process_init(s, current, stop, pipe_fd);
	signal(SIGINT, &sig_int);
	if (close(pipe_fd[STDOUT_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	return (0);
}