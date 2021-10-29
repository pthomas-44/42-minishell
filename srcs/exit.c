/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:24:04 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/29 14:13:00 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// //~~ print les erreurs types du bash

// void	print_error(char *prompt, char *value, char *error)
// {
// 	write(2, prompt, ft_strlen(prompt));
// 	write(2, value, ft_strlen(value));
// 	write(2, error, ft_strlen(value));
// }

//~~ Free la structure t_cmds

void	free_cmds_struct(t_structs *s)
{
	size_t	i;

	i = 0;
	while (s->cmds && i < s->cmds_size)
	{
		free_tab(s->cmds[i].cmd, 0);
		free(s->cmds[i].path);
		i++;
	}
	free(s->cmds);
	s->cmds = NULL;
	s->cmds_size = 0;
}

//~~ Affiche les message d'erreur et libere toute la memoire allouee

void	ft_exit(t_structs *s, char *errormsg, int status)
{
	if (status == EXIT_SUCCESS || !ft_strcmp(errormsg, "exit"))
		write(1, "exit\n", 6);
	else
		perror(errormsg);
	free(s->parse_line[0]);
	s->parse_line[0] = NULL;
	free(s->parse_line[1]);
	free_cmds_struct(s);
	env_clear(s);
	rl_clear_history();
	// tcsetattr(STDIN_FILENO, TCSANOW, &s->term.basic);
	exit(status);
}
