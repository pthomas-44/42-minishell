/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:24:04 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/15 11:29:14 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
}

//~~ Affiche les message d'erreur et free la memoire allouee

void	ft_exit(t_structs *s, char *errormsg, int status)
{
	if (status != EXIT_SUCCESS)
		perror(errormsg);
	else if (status == EXIT_SUCCESS)
		write(1, "exit\n", 5);
	else if (status == EXIT_MISSING)
		write(2, errormsg, ft_strlen(errormsg));
	free_cmds_struct(s);
	env_clear(s);
	rl_clear_history();
	exit(status);
}
