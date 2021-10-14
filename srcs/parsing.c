/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/14 13:42:00 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Remplis la structure t_cmd avec les donnees de line

int	heredoc_handler(t_structs *s, char *stop, int i)
{
	int		pipe_fd[2];
	char	*content;

	content = heredoc_loop(stop);
	if (!content)
		return (-1);
	pipe(pipe_fd);
	if (pipe_fd[0] == -1 || pipe_fd[1] == -1)
	{
		perror("pipe");
		return (-1);
	}
	write(pipe_fd[STDOUT_FILENO], content, ft_strlen(content));
	if (close(pipe_fd[STDIN_FILENO]) == -1)
	{
		perror("close");
		return (-1);
	}
	s->cmds[i].fd_in = pipe_fd[1];
	return (0);
}

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
			i++;
		}
		else if (*line && get_command(s, &line, i) == -1)
			return (-1);
	}
	//split_quotes
	return (0);
}

//~~ Remplace les variables d'environnement pas leurs valeurs

void	replace_env_variables(t_structs *s)
{
	(void)s;
	return ;
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
	if (*line && syntax_loop(line, charset, &quote, &last_char))
		return (1);
	if (*line && ft_strchr(charset, last_char))
	{
		write(2, "minishell: syntax error near \
unexpected token `newline'\n", 57);
		return (1);
	}
	if (*line && quote)
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
