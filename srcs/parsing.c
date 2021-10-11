/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 15:43:01 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


//~~ Remplis la structure t_cmd avec les donnees de line

int	fill_cmd_struct(t_structs *s, char *line)
{
	size_t	i;

	i = 0;
	while (*line)
	{
		skip_spaces(&line);
		if (*line == '<' && get_infile(s, &line, i) == -1)
			return (-1);
		else if (*line == '>' && get_outfile(s, &line, i) == -1)
			return (-1);
		else if (*line == '|')
		{
			line++;
			printf("%s\n", s->cmds[i].cmd[0]);
			i++;
		}
		else if (*line && get_command(s, &line, i) == -1)
			return (-1);
	}
	printf("%s\n", s->cmds[i].cmd[0]);
	return (0);
}

//~~ Remplace les variables d'environnement pas leurs valeurs

void	replace_env_variables(t_structs *s)
{
	(void)s;
	return ;
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
		else if (ft_strchr(charset, *line) && *quote == 0
			&& *(line + 1) == *line && (*(line + 2) == *line || *line == '|'))
		{
			write(2, "1minishell: syntax error near unexpected token `", 48);
			write(2, line, 1);
			write(2, "'\n", 3);
			return (1);
		}
		line++;
	}
	return (0);
}

//~~ Verifie les erreurs de syntax

int	check_syntax_errors(char *line, char *charset)
{
	char	quote;
	char	last_char;

	skip_spaces(&line);
	if (*line == '|')
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	if (syntax_loop(line, charset, &quote, &last_char))
		return (1);
	if (ft_strchr(charset, last_char))
	{
		write(2, "minishell: syntax error near \
unexpected token `newline'\n", 57);
		return (1);
	}
	if (quote)
	{
		write(2, "minishell: unclosed quotes\n", 28);
		return (1);
	}
	return (0);
}

//~~ Le parsing, qui appel "ta fonction" avec un tableau de structure t_cmd 

void	parsing(t_structs *s, char *line)
{
	if (!(*line) || check_syntax_errors(line, "<>|"))
		return ;
	init_cmds_struct(s, line);
	replace_env_variables(s);
	if (fill_cmd_struct(s, line) == -1)
	{
		free_cmds_struct(s);
		return ;
	}
	//execution
	free_cmds_struct(s);
}
