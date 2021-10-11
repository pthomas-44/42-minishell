/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:54:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 15:42:54 by pthomas          ###   ########lyon.fr   */
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
	return (nb_of_pipes + 1);
}

void	init_cmds_struct(t_structs *s, char *line)
{
	size_t	i;

	i = 0;
	s->cmds_size = nb_of_pipes(line);
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
