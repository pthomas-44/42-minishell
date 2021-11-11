/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:26:00 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/11 11:31:20 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Recuparation du fichier de redirection de la sortie

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_out != 1 && close(s->cmds[i].fd_out) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	tmp[0] = *line;
	while (*(*line) == '>')
		(*line)++;
	skip_spaces(&(*line));
	tmp[1] = get_args(*line, "<>| ");
	if (*(tmp[0] + 1) == '>')
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (s->cmds[i].fd_out == -1)
	{
		print_error("open: ", tmp[1], NULL, errno);
		free(tmp[1]);
		return (-1);
	}
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}

//~~ Suite de la fonction get_infile() (norme)

static int	get_infile_sequel(t_structs *s, int i, char **tmp)
{
	s->cmds[i].fd_in = open(tmp[1], O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		print_error("open: ", tmp[1], NULL, errno);
		s->cmds[i].fd_in = -1;
	}
	return (0);
}

//~~ Recuparation du fichier de redirection de l'entree

int	get_infile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_in != 0 && close(s->cmds[i].fd_in) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	tmp[0] = *line;
	while (*(*line) == '<')
		(*line)++;
	skip_spaces(line);
	tmp[1] = get_args(*line, "<>| ");
	if (*(tmp[0] + 1) == '<')
	{
		if (heredoc_handler(s, &s->cmds[i], tmp[1]) == -1)
		{
			free(tmp[1]);
			return (-1);
		}
	}
	else if (get_infile_sequel(s, i, tmp) == -1)
		return (-1);
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}
