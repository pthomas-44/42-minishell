/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/18 13:03:29 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Gere l'operateur '<<' comme un heredoc

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
		free(content);
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
			s->cmds[i].cmd = ft_split_quotes(s->cmds[i].cmd[0]);
			remove_quotes(&s->cmds[i].cmd);
			line++;
			i++;
		}
		else if (*line && get_command(s, &line, i) == -1)
			return (-1);
	}
	s->cmds[i].cmd = ft_split_quotes(s->cmds[i].cmd[0]);
	remove_quotes(&s->cmds[i].cmd);
	return (0);
}

//~~ Remplace les variables d'environnement pas leurs valeurs

char	*replace_env_variables(t_structs *s, char *line)
{
	t_env	*var;
	size_t	i;
	char	quote;
	char	*new;

	i = 0;
	quote = 0;
	new = NULL;
	while (line && line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		if (line[i] == '$' && line[i + 1] != ' '
			&& line[i + 1] != '?' && quote != '\'')
		{
			var = find_var(s, &line[i + 1]);
			new = replace_var(line, i, var);
		}
		i++;
	}
	if (!new)
		new = ft_strdup(line);
	return (new);
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

void	print_args(t_structs *s)
{
	size_t i = 0;
	size_t j = 0;

	while (j < s->cmds_size)
	{
		i = 0;
		while (s->cmds->cmd[i])
		{
			printf("cmd %zu | arg %zu | = |%s| ", j, i, s->cmds->cmd[i]);
			i++;
		}
		printf("\n");
		j++;
	}
}

//~~ Le parsing, qui appel "ta fonction" avec un tableau de structure t_cmd 

void	parsing(t_structs *s, char *line)
{
	char	*tmp;

	if (!(*line) || check_syntax_errors(line, "<>|"))
		return ;
	init_cmds_struct(s, line);
	tmp = replace_env_variables(s, line);
	if (!tmp)
		return ;
	if (fill_cmd_struct(s, tmp) == -1)
	{
		free(tmp);
		free_cmds_struct(s);
		return ;
	}
	free(tmp);
	exec_cmds(s);
	free_cmds_struct(s);
}
