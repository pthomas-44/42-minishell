/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/15 13:00:09 by pthomas          ###   ########lyon.fr   */
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
	free(content);
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
	return (0);
}

// char	*replace_var()

// char	*find_value(t_structs *s, char *line)
// {
// 	t_env	*current;
// 	size_t	i;

// 	i = 0;
// 	while (line[i] && line[i] != ' ' && line[i] != '"')
// 		i++;
// 	line[i] = 0;
// 	current = *s->env;
// 	while (ft_strcmp(current->name, line))
// 		current = current->next;
// 	if (ft_strcmp(current->name, line))
// 		return ("");
// 	else
// 		return (current->value);
// }
//~~ Remplace les variables d'environnement pas leurs valeurs

char	*replace_env_variables(t_structs *s, char *line)
{
	(void)s;
	(void)line;
	// char	*value;
	// size_t	i;
	// char	quote;

	// i = 0;
	// while (line && line[i])
	// {
	// 	if ((line[i] == '"' || line[i] == '\'') && quote == 0)
	// 		quote = line[i];
	// 	else if (line[i] == quote)
	// 		quote = 0;
	// 	if (line[i] == '$' && line[i + 1] != ' ' && quote != '\'')
	// 	{
	// 		value = find_value(s, &line[i + 1]);
	// 		line = replace_var(line, i);
	// 	}
	// 	i++;
	// }
	return (line);
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
	size_t	i;
	char	*tmp;

	if (!(*line) || check_syntax_errors(line, "<>|"))
		return ;
	init_cmds_struct(s, line);
	line = replace_env_variables(s, line);
	if (!line)
		return ;
	if (fill_cmd_struct(s, line) == -1)
	{
		free_cmds_struct(s);
		return ;
	}
	i = 0;
	while (i < s->cmds_size)
	{
		tmp = s->cmds[i].cmd[0];
		s->cmds[i].cmd = ft_split_quotes(s->cmds[i].cmd[0]);
		i++;
		free(tmp);
	}
	//execution
	free_cmds_struct(s);
}
