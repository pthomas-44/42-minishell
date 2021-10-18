/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:26:00 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/18 15:27:09 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_out > 1 && close(s->cmds[i].fd_out) == -1)
	{
		perror("close");
		return (-1);
	}
	while (*(*line) == '>')
		(*line)++;
	tmp[0] = *line;
	skip_spaces(&(*line));
	tmp[1] = get_args(*line, 1);
	if (*(tmp[0] - 2) == '>')
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_RDWR, 0644);
	else
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (s->cmds[i].fd_out == -1)
	{
		free(tmp[1]);
		perror("open");
		return (-1);
	}
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}

int	get_infile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_in > 0 && close(s->cmds[i].fd_in) == -1)
	{
		perror("close");
		return (-1);
	}
	while (*(*line) == '<')
		(*line)++;
	tmp[0] = *line;
	skip_spaces(line);
	tmp[1] = get_args(*line, 1);
	if (*(tmp[0] - 2) == '<')
	{
		if (heredoc_handler(s, tmp[1], i) == -1)
		{
			free(tmp[1]);
			return (-1);
		}
	}
	if (get_infile_sequel(s, &line, i, tmp) == -1)
		return (-1);
	return (0);
}

int	get_infile_sequel(t_structs *s, char ***line, int i, char **tmp)
{
	if (*(tmp[0] - 2) != '<')
		s->cmds[i].fd_in = open(tmp[1], O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		free(tmp[1]);
		perror("open");
		return (-1);
	}
	free(tmp[1]);
	(*(*line)) += ft_strlen(tmp[1]);
	return (0);
}

int	heredoc_handler(t_structs *s, char *stop, int i)
{
	int		pipe_fd[2];
	char	*content;

	content = heredoc_loop(stop);
	if (!content)
		return (-1);
	pipe(pipe_fd);
	if (pipe_fd[0] == -1 || pipe_fd[1] == -1)
	{
		perror("pipe");
		free(content);
		return (-1);
	}
	write(pipe_fd[STDOUT_FILENO], content, ft_strlen(content));
	free(content);
	if (close(pipe_fd[STDIN_FILENO]) == -1)
	{
		perror("close");
		return (-1);
	}
	s->cmds[i].fd_in = pipe_fd[1];
	return (0);
}

char	*heredoc_loop(char *stop)
{
	char	*line;
	char	*content;

	content = NULL;
	while (ft_strcmp(line, stop))
	{
		line = readline(HEREDOC_PROMPT);
		if (!line)
			break ;
		if (content)
			content = ft_strjoin_f1(content, "\n");
		content = ft_strjoin_f3(content, line);
		if (!content)
			break ;
	}
	return (content);
}
