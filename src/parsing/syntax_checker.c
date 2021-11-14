/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:24:16 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/14 11:50:08 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Passe des espaces successifs dans une chaine de caracteres

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

//~~ Verifie si les operateurs se suivent

static int	check_successive_operators(char **line, char *charset)
{
	char	*tmp;
	char	*tmp2;

	tmp = (*line) + 1;
	skip_spaces(&tmp);
	if (*tmp)
	{
		tmp2 = tmp + 1;
		skip_spaces(&tmp2);
	}
	else
		tmp2 = NULL;
	if (*tmp && *tmp == '|')
		print_error(NULL, NULL, "syntax error near unexpected token `|'\n", 258);
	else if (*tmp && ft_strchr(charset, *tmp) && *(*line) != '|'
		&& (*tmp != *(*line) || (*tmp2 && ft_strchr(charset, *tmp2))))
	{
		print_error(NULL, NULL,
			"syntax error near unexpected token `", 258);
		write(STDERR_FILENO, tmp, 1);
		write(STDERR_FILENO, "'\n", 2);
	}
	else
		return (0);
	return (-1);
}

//~~ Verifie chaque caracteres

static int	syntax_loop(char *line, char *charset, char *quote, char *last_char)
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
		else if (ft_strchr(charset, *line) && *quote == 0)
			if (check_successive_operators(&line, charset) == -1)
				return (-1);
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
		print_error(NULL, NULL, "syntax error near unexpected token `|'\n", 258);
		return (1);
	}
	if (*line && syntax_loop(line, charset, &quote, &last_char) == -1)
		return (1);
	if (*line && ft_strchr(charset, last_char))
	{
		print_error(NULL, NULL,
			"syntax error near unexpected token `newline'\n", 258);
		return (1);
	}
	if (*line && quote)
	{
		print_error(NULL, NULL, "unclosed quotes\n", 258);
		return (1);
	}
	return (0);
}
