/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:26:00 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/14 14:26:39 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Recuparation du nom du fichier

static char	*get_filename(char **line, bool *is_double)
{
	char	*filename;

	if (*((*line) + 1) == *(*line))
	{
		*is_double = 1;
		(*line)++;
	}
	(*line)++;
	skip_spaces(line);
	filename = get_args(*line, "<>| ");
	(*line) += ft_strlen(filename);
	remove_quotes_and_backslash(&filename);
	return (filename);
}

//~~ Recuparation du fichier de redirection de la sortie

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*filename;
	bool	is_double;

	is_double = 0;
	if (s->cmds[i].fd_out != 1 && close(s->cmds[i].fd_out) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	filename = get_filename(line, &is_double);
	if (is_double)
		s->cmds[i].fd_out = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		s->cmds[i].fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (s->cmds[i].fd_out == -1)
	{
		print_error("open: ", filename, NULL, errno);
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

//~~ Recuparation du fichier de redirection de l'entree

int	get_infile(t_structs *s, char **line, int i)
{
	char	*filename;
	bool	is_double;

	is_double = 0;
	if (s->cmds[i].fd_in > 0 && close(s->cmds[i].fd_in) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	filename = get_filename(line, &is_double);
	if (is_double)
	{
		if (heredoc_handler(s, &s->cmds[i], filename) == -1)
			return (-1);
	}
	else
		s->cmds[i].fd_in = open(filename, O_RDONLY);
	if (s->cmds[i].fd_in == -1)
		print_error("open: ", filename, NULL, errno);
	free(filename);
	return (0);
}
