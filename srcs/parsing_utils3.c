/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 15:41:41 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/14 13:41:50 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*get_args(char *line, bool is_file)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && (((!is_file || *line != ' ' )
				&& *line != '<' && *line != '>' && *line != '|') || quote))
	{
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		line++;
	}
	return (ft_substr(start, 0, line - start));
}

int	get_infile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_in > -1 && close(s->cmds[i].fd_in) == -1)
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
		if (heredoc_handler(s, tmp[1], i) == -1)
			return (-1);
	if (*(tmp[0] - 2) != '<')
		s->cmds[i].fd_in = open(tmp[1], O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		perror("open");
		return (-1);
	}
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_out > -1 && close(s->cmds[i].fd_out) == -1)
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
		perror("open");
		return (-1);
	}
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}

int	get_command(t_structs *s, char **line, int i)
{
	char	*tmp;

	tmp = NULL;
	tmp = get_args(*line, 0);
	if (!s->cmds[i].cmd)
	{
		s->cmds[i].cmd = malloc(sizeof(char *) * 2);
		if (!s->cmds[i].cmd)
		{
			perror("malloc");
			return (-1);
		}
		s->cmds[i].cmd[0] = ft_strdup(tmp);
		s->cmds[i].cmd[1] = NULL;
	}
	else
		s->cmds[i].cmd[0] = ft_strjoin_f1(s->cmds[i].cmd[0], tmp);
	(*line) += ft_strlen(tmp);
	free(tmp);
	return (0);
}
