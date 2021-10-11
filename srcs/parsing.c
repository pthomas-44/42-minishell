/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 13:35:07 by pthomas          ###   ########lyon.fr   */
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
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
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
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		line++;
	}
	return (ft_substr(start, 0, line - start));
}

//~~ Remplis la structure t_cmd avec les donnees de line mauvaise gestion d'erreur

void	fill_cmd_struct(t_structs *s, char *line)
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
			if (s->cmds[i].fd_in > -1 && close(s->cmds[i].fd_in) == -1)
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
			s->cmds[i].fd_in = open(tmp, O_RDONLY);
			if (s->cmds[i].fd_in == -1)
				ft_exit(s, "open", EXIT_FAILURE);
			// }
			line += ft_strlen(tmp);
			free(tmp);
		}
		else if (*line == '>')
		{
			printf("%zu | %d\n", i, s->cmds[i].fd_out);
			if (s->cmds[i].fd_out > -1 && close(s->cmds[i].fd_out) == -1)
			{
				ft_exit(s, "open", EXIT_FAILURE);
			}
			if (*(line + 1) == '>')
			{
				line += 2;
				skip_spaces(&line);
				tmp = get_file(line);
				s->cmds[i].fd_out = open(tmp, O_CREAT | O_RDWR);
				if (s->cmds[i].fd_out == -1)
					ft_exit(s, "open", EXIT_FAILURE);
			}
			else
			{
				line++;
				skip_spaces(&line);
				tmp = get_file(line);
				s->cmds[i].fd_out = open(tmp, O_CREAT | O_RDWR | O_TRUNC);
				if (s->cmds[i].fd_out == -1)
					ft_exit(s, "open", EXIT_FAILURE);
			}
			line += ft_strlen(tmp);
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
			// ft_strjoin_f1(s->cmds[i].cmd[0], tmp);
			line += ft_strlen(tmp);
			free(tmp);
		}
	}
}

//~~ Verifie les erreurs de syntax

int	check_syntax_errors(char *line)
{
	char	quote;
	char	last_char;

	quote = 0;
	last_char = 0;
	skip_spaces(&line);
	if (*line == '|')
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	while (*line)
	{
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		else if (*line == '|' && *(line + 1) == '|')
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 51);
			return (1);
		}
		else if (*line == '<' && *(line + 1) == '<' && *(line + 2) == '<')
		{
			write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
			return (1);
		}
		else if (*line == '>' && *(line + 1) == '>' && *(line + 2) == '>')
		{
			write(2, "minishell: syntax error near unexpected token `>>'\n", 52);
			return (1);
		}
		if (*line != ' ')
			last_char = *line;
		line++;
	}
	if (last_char == '|' || last_char == '<' || last_char == '>')
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
		return (1);
	}
	if (quote)
	{
		write(2, "minishell: unclosed quotes\n", 28);
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
	return (nb_of_pipes + 1);
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
		s->cmds[i].fd_in = -1;
		s->cmds[i].cmd = NULL;
		s->cmds[i].path = NULL;
		s->cmds[i].fd_out = -1;
		s->cmds[i].pid = -1;
		i++;
	}
}

//~~ Le parsing, qui appel "ta fonction" avec un tableau de structure t_cmd 

void	parsing(t_structs *s, char *line)
{
	if (check_syntax_errors(line))
		return ;
	init_cmds_struct(s, line);
	replace_env_variables(s);
	fill_cmd_struct(s, line);
	// printf("line : %s\n", line);
	//execution
	free_cmds_struct(s);
}
