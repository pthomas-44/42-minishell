/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/06 19:58:22 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Remplace les variables d'environnement pas leurs valeurs

void	replace_env_variables(t_structs *s)
{
	(void)s;
	return ;
}

//~~ Remplis la structure t_cmd avec les donnees de line

void	fill_cmd_struct(t_structs *s, t_cmd *cmd, char *line)
{
	(void)s;
	(void)cmd;
	(void)line;
	return ;
}

//~~ Verifie les caracteres interdit et les quotes non fermees

int	is_wrong_chars(char *line)
{
	(void)line;
	return (0);
}

//~~ compte le nombre de pipe pour malloc t_cmd

int	nb_of_pipes(char *line)
{
	(void)line;
	return (0);
}

//~~ Le parsing, qui appel "ta fonction" avec un tableau de structure t_cmd 

void	parsing(t_structs *s, char *line)
{
	s->cmds = malloc(sizeof(t_cmd) * nb_of_pipes(line) + 1);
	if (!s->cmds)
		ft_exit(s, "malloc", EXIT_FAILURE);
	if (is_wrong_chars(line))
	{
		write(1, "error: some chars in entry are not supported\n", 46);
		return ;
	}
	fill_cmd_struct(s, s->cmds, line);
	replace_env_variables(s);
}
	// execution