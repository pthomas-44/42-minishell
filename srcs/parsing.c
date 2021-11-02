/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/02 14:22:19 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Recupere la ligne de commande

static int	get_command(t_structs *s, char **line, int i)
{
	char	*tmp;

	tmp = get_args(*line, 0);
	if (tmp && !s->cmds[i].cmd)
	{
		s->cmds[i].cmd = ft_split(tmp, 0);
		if (!s->cmds[i].cmd)
		{
			free(tmp);
			print_error("malloc: ", NULL, NULL, ENOMEM);
			return (-1);
		}
	}
	else if (tmp)
		s->cmds[i].cmd[0] = ft_strjoin_f1(s->cmds[i].cmd[0], tmp);
	if (!s->cmds[i].cmd[0])
	{
		free(tmp);
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (-1);
	}
	(*line) += ft_strlen(tmp);
	free(tmp);
	return (0);
}

//~~ Remplis la structure t_cmd avec les donnees de la ligne de commandes

static int	fill_cmd_struct(t_structs *s, char *line)
{
	size_t	i;

	i = 0;
	while (*line)
	{
		skip_spaces(&line);
		if (*line && !ft_strchr("<>|", *line) && get_command(s, &line, i) == -1)
			return (-1);
		else if (*line == '<' && get_infile(s, &line, i) == -1)
			return (-1);
		else if (*line == '>' && get_outfile(s, &line, i) == -1)
			return (-1);
		else if (*line == '|')
		{
			s->cmds[i].cmd = split_cmd(s->cmds[i].cmd);
			remove_quotes(s->cmds[i].cmd);
			line++;
			i++;
		}
	}
	s->cmds[i].cmd = split_cmd(s->cmds[i].cmd);
	remove_quotes(s->cmds[i].cmd);
	return (0);
}

// Compte le nombre de pipes dans la ligne de commandes

static size_t	nb_of_pipes(char *line)
{
	size_t	nb_of_pipes;
	char	quote;

	nb_of_pipes = 0;
	quote = 0;
	while (*line)
	{
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		else if (*line == '|' && !quote)
			nb_of_pipes++;
		line++;
	}
	return (nb_of_pipes);
}

// Initialisation de la structure t_cmd

static int	init_cmds_struct(t_structs *s, char *line)
{
	size_t	i;

	s->cmds_size = nb_of_pipes(line) + 1;
	s->cmds = ft_calloc(s->cmds_size, sizeof(t_cmd));
	if (!s->cmds)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (-1);
	}
	i = 0;
	while (i < s->cmds_size)
	{
		s->cmds[i].fd_in = 0;
		s->cmds[i].cmd = NULL;
		s->cmds[i].path = NULL;
		s->cmds[i].fd_out = 1;
		i++;
	}
	return (0);
}

//~~ Le parsing, qui appel l'execution avec un tableau 
//		de structures t_cmd contenant les donnees du parsing

void	parsing(t_structs *s, char *line)
{
	char	*tmp;

	if (check_syntax_errors(line, "<>|") || init_cmds_struct(s, line) == -1)
		return ;
	tmp = replace_env_variables(s, ft_strdup(line));
	// printf("%s\n", (*s->env)->value);
	if (!tmp)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return ;
	}
	if (fill_cmd_struct(s, tmp) == -1)
	{
		free(tmp);
		free_cmds_struct(s);
		return ;
	}
	// printf("%s\n", s->cmds->cmd[0]);
	free(tmp);
	exec(s);
	free_cmds_struct(s);
}
