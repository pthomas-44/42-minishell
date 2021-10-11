/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 15:41:41 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 15:42:46 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_file(char *line)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && ((*line != ' ' && *line != '<'
				&& *line != '>' && *line != '|') || quote))
	{
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		line++;
	}
	return (ft_substr(start, 0, line - start));
}

char	*get_cmd(char *line)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && ((*line != '<' && *line != '>' && *line != '|') || quote))
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
	char	*tmp;

	tmp = NULL;
	if (s->cmds[i].fd_in > -1 && close(s->cmds[i].fd_in) == -1)
	{
		perror("open");
		return (-1);
	}
	// if ((*(*line)) + 1 == '<')
	// {
	// 	(*line) += 2;
	// 	skip_spaces(line);
	// 	heredoc
	// }
	// else
	// {
	(*line)++;
	skip_spaces(line);
	tmp = get_file(*line);
	s->cmds[i].fd_in = open(tmp, O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		perror("open");
		return (-1);
	}
	// }
	(*line) += ft_strlen(tmp);
	free(tmp);
	return (0);
}

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*tmp;

	tmp = NULL;
	if (s->cmds[i].fd_out > -1 && close(s->cmds[i].fd_out) == -1)
	{
		perror("open");
		return (-1);
	}
	while (*(*line) == '>')
		(*line)++;
	skip_spaces(&(*line));
	tmp = get_file((*line));
	if ((*(*line)) + 1 == '>')
		s->cmds[i].fd_out = open(tmp, O_CREAT | O_RDWR, 0644);
	else
		s->cmds[i].fd_out = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (s->cmds[i].fd_out == -1)
	{
		perror("open");
		return (-1);
	}
	(*line) += ft_strlen(tmp);
	free(tmp);
	return (0);
}

int	get_command(t_structs *s, char **line, int i)
{
	char	*tmp;

	tmp = NULL;
	tmp = get_cmd((*line));
	if (!s->cmds[i].cmd)
	{
		s->cmds[i].cmd = malloc(sizeof(char *) * 2);
		if (!s->cmds[i].cmd)
		{
			perror("open");
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