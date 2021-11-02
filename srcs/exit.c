/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:24:04 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/02 12:38:56 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// //~~ Print les erreurs types du bash

void	print_error(char *cmd, char *value, char *error, int status)
{
	errno = status;
	write(STDERR_FILENO, "potatoshell: ", 13);
	if (cmd)
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
	if (value)
	{
		write(STDERR_FILENO, value, ft_strlen(value));
		write(STDERR_FILENO, ": ", 2);
	}
	if (errno != 258 && errno != EXIT_FAILURE
		&& errno != 127 && errno != 255 && errno != 3)
		perror(NULL);
	if (error)
		write(STDERR_FILENO, error, ft_strlen(error));
}

//~~ Free la structure t_cmds

void	free_cmds_struct(t_structs *s)
{
	size_t	i;

	i = 0;
	while (s->cmds && i < s->cmds_size)
	{
		free_tab(s->cmds[i].cmd, 0);
		free(s->cmds[i].path);
		s->cmds[i].path = NULL;
		i++;
	}
	free(s->cmds);
	s->cmds = NULL;
	s->cmds_size = 0;
}

//~~ Affiche les messages d'erreur et libere toute la memoire allouee

void	free_all(t_structs *s)
{
	// tcsetattr(STDIN_FILENO, TCSANOW, &(s->term.basic));
	free(s->parse_line[0]);
	s->parse_line[0] = NULL;
	free(s->parse_line[1]);
	s->parse_line[1] = NULL;
	free_cmds_struct(s);
	env_clear(s);
	rl_clear_history();
}
