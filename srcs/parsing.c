/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 11:49:03 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Remplace les variables d'environnement pas leurs valeurs

void	replace_env_variables(t_structs *s)
{
	(void)s;
	return ;
}

//~~ Passe les espaces

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

//~~ Recupere le nom d'un fichier apres un caracteres '<' ou '>'

char	*get_file(char *line)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && (*line != ' ' || (*line == ' ' && quote)))
	{
		if (*line == '"' && quote == 0)
			quote = '"';
		else if (*line == '\'' && quote == 0)
			quote = '\'';
		else if (*line == quote)
			quote = 0;
		line++;
	}
	return (ft_substr(start, 0, line - start));
}

//~~ Recupere la commande et ses arguments

char	*get_cmd(char *line)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && ((*line != '<' && *line != '>' && *line != '|') || quote))
	{
		if (*line == '"' && quote == 0)
			quote = '"';
		else if (*line == '\'' && quote == 0)
			quote = '\'';
		else if (*line == quote)
			quote = 0;
		line++;
	}
	return (ft_substr(start, 0, line - start));
}

//~~ Remplis la structure t_cmd avec les donnees de line mauvaise gestion d'erreur

void	fill_cmd_struct(t_structs *s, t_cmd *cmds, char *line)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (*line)
	{
		skip_spaces(&line);
		if (*line == '<')
		{
			if (cmds[i].fd_in > -1 && close(cmds[i].fd_in) == -1)
				ft_exit(s, "open", EXIT_FAILURE);
			// if (*(line + 1) == '<')
			// {
			// 	line += 2;
			// 	skip_spaces(&line);
			// 	heredoc
			// }
			// else
			// {
			line++;
			skip_spaces(&line);
			tmp = get_file(line);
			cmds[i].fd_in = open(tmp, O_RDONLY);
			if (cmds[i].fd_in == -1)
				ft_exit(s, "open", EXIT_FAILURE);
			// }
			free(tmp);
		}
		else if (*line == '>')
		{
			if (cmds[i].fd_out > -1 && close(cmds[i].fd_out) == -1)
				ft_exit(s, "open", EXIT_FAILURE);
			if (*(line + 1) == '>')
			{
				line += 2;
				skip_spaces(&line);
				tmp = get_file(line);
				cmds[i].fd_out = open(tmp, O_CREAT);
			}
			else
			{
				line++;
				skip_spaces(&line);
				tmp = get_file(line);
				cmds[i].fd_out = open(tmp, O_CREAT | O_TRUNC);
			}
			free(tmp);
		}
		else if (*line == '|')
		{
			line++;
			i++;
		}
		else if (*line)
		{
			tmp = get_cmd(line);
			line += ft_strlen(tmp);
			free(tmp);
		}
	}
}

//~~ Verfie qu'il n'y a pas de pipe vide

int	check_empty_pipes(char *line)
{
	char	pipe;

	pipe = 0;
	while (*line)
	{
		skip_spaces(&line);
		if (*line == pipe)
		{
			write(2, "minishell: syntax error near unexpected token '|'\n", 51);
			return (1);
		}
		else if (*line == '|' && !pipe)
			pipe = '|';
		else if (pipe && *line != pipe)
			pipe = 0;
		line ++;
	}
	return (0);
}

//~~ Verifie les quotes non fermees

int	check_unclosed_quotes(char *line)
{
	char	quote;

	quote = 0;
	while (*line)
	{
		if (*line == '"' && quote == 0)
			quote = '"';
		else if (*line == '\'' && quote == 0)
			quote = '\'';
		else if (*line == quote)
			quote = 0;
		line++;
	}
	if (quote)
	{
		write(2, "minishell: unclosed quotes\n", 23);
		return (1);
	}
	return (0);
}

//~~ Compte le nombre de pipe pour malloc t_cmd

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

//~~ Initialise la structure t_cmds à NULL

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
		s->cmds->fd_in = -1;
		s->cmds->cmd = NULL;
		s->cmds->path = NULL;
		s->cmds->fd_out = -1;
		s->cmds->pid = -1;
		i++;
	}
}

//~~ Le parsing, qui appel "ta fonction" avec un tableau de structure t_cmd 

void	parsing(t_structs *s, char *line)
{
	init_cmds_struct(s, line);
	if (check_unclosed_quotes(line) || check_empty_pipes(line))
		return ;
	replace_env_variables(s);
	// fill_cmd_struct(s, s->cmds, line);
	printf("%s\n", line);
	//execution
	free(s->cmds);
}
