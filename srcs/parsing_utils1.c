/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:54:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/14 13:38:28 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

size_t	nb_of_pipes(char *line)
{
	size_t	nb_of_pipes;

	nb_of_pipes = 0;
	while (*line)
	{
		if (*line == '|')
			nb_of_pipes++;
		line++;
	}
	return (nb_of_pipes);
}

void	init_cmds_struct(t_structs *s, char *line)
{
	size_t	i;

	i = 0;
	s->cmds_size = nb_of_pipes(line) + 1;
	s->cmds = malloc(sizeof(t_cmd) * s->cmds_size);
	if (!s->cmds)
		ft_exit(s, "malloc", EXIT_FAILURE);
	ft_bzero(s->cmds, sizeof(t_cmd) * s->cmds_size);
	while (i < s->cmds_size)
	{
		s->cmds[i].fd_in = -1;
		s->cmds[i].cmd = NULL;
		s->cmds[i].cmd = NULL;
		s->cmds[i].path = NULL;
		s->cmds[i].fd_out = -1;
		s->cmds[i].pid = -1;
		i++;
	}
}

int	check_successive_operators(char **line, char *charset)
{
	char	*tmp;

	tmp = (*line) + 1;
	skip_spaces(&tmp);
	if (*tmp && ft_strchr(charset, *tmp) && (*(*line) != '|' || *tmp == '|')
		&& (*tmp != *(*line) || tmp - 1 != (*line)))
	{
		write(2, "minishell: syntax error near unexpected token `", 48);
		write(2, tmp, 1);
		write(2, "'\n", 3);
		return (-1);
	}
	else if (*tmp)
	{
		tmp++;
		skip_spaces(&tmp);
		if (*tmp && ft_strchr(charset, *tmp))
		{
			write(2, "minishell: syntax error near unexpected token `", 48);
			write(2, tmp, 1);
			write(2, "'\n", 3);
			return (-1);
		}
	}
	return (0);
}

int	syntax_loop(char *line, char *charset, char *quote, char *last_char)
{
	*quote = 0;
	*last_char = 0;
	while (*line)
	{
		if (*line != ' ')
			*last_char = *line;
		if ((*line == '"' || *line == '\'') && *quote == 0)
			*quote = *line;
		else if (*line == *quote)
			*quote = 0;
		else if (ft_strchr(charset, *line) && *quote == 0)
			if (check_successive_operators(&line, charset) == -1)
				return (-1);
		line++;
	}
	return (0);
}
